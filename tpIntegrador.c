// https://github.com/usuario/tp-telegram-bot

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // sleep(2)
#include <curl/curl.h>



struct memory {
    char *response;
    size_t size;
};

// Callback: libcurl llama a esta funcion cada vez que recibe datos 
static size_t cb(char *data, size_t size, size_t nmemb, void *clientp)
{
    size_t realsize = size * nmemb;
    struct memory *mem = (struct memory *)clientp;

    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if (!ptr)
        return 0;  //no hay memoria

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = '\0';

    return realsize;
}

//funcion que ereemplaza espacios por %20
void encode_spaces(const char *texto, char *dest, size_t max)
{
    size_t j = 0;
    for (size_t i = 0; texto[i] != '\0' && j < max - 1; i++) {
        if (texto[i] == ' ') {
            if (j + 3 >= max) break;
            dest[j++] = '%';
            dest[j++] = '2';
            dest[j++] = '0';
        } else {
            dest[j++] = texto[i];
        }
    }
    dest[j] = '\0';
}

//pasa texto a minusculas
void a_minusculas(const char *src, char *dst, size_t max)
{
    size_t i;
    for (i = 0; src[i] != '\0' && i < max - 1; i++) {
        char c = src[i];
        if (c >= 'A' && c <= 'Z') {
            c = c - 'A' + 'a';
        }
        dst[i] = c;
    }
    dst[i] = '\0';
}

int main(int argc, char *argv[])
{


    printf("Arranca el programa\n");

    //LEER TOKEN DEL BOT DESDE ARCHIVO POR CUESTIONES DE SEGURIDAD
    if (argc < 2) {
        printf("Uso: %s archivo_token.txt\n", argv[0]);
        return 1;
    }

    char token[128];
    FILE *ft = fopen(argv[1], "r");
    if (ft == NULL) {
        printf("No se pudo abrir el archivo de token\n");
        return 1;
    }

    if (fgets(token, sizeof(token), ft) == NULL) {
        printf("No se pudo leer el token\n");
        fclose(ft);
        return 1;
    }
    fclose(ft);

    // sacar \n del final si quedo
    token[strcspn(token, "\r\n")] = '\0';


    //crear la URL
    char base_url[256];
    snprintf(base_url, sizeof(base_url),
             "https://api.telegram.org/bot%s", token);

    //archivo log historial de chats
    FILE *log = fopen("bot.log", "a");
    if (log == NULL) {
        printf("No se pudo abrir bot.log\n");
        return 1;
    }

    long last_update_id = 0;   // ultimo update procesado

    //loop consultando nuevos mensajes cada dos segundos
    while (1) {
        char api_url[512];

        if (last_update_id == 0) {
            //primera vez que se ejecuta pide todos los msj disponibles
            snprintf(api_url, sizeof(api_url),
                     "%s/getUpdates", base_url);
        } else {
            //solo lee mensajes nuevos
            snprintf(api_url, sizeof(api_url),
                     "%s/getUpdates?offset=%ld", base_url, last_update_id + 1);
        }

        CURL *curl = curl_easy_init();
        struct memory chunk = {0};
        CURLcode res;

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, api_url); //que URL usar
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb); //funcion para guardar los datos
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk); //donde se guardan los datos
 
            // para que wwindows no moleste con la seguridad 
            curl_easy_setopt(curl, CURLOPT_CAINFO, "C:\\curl\\cacert.pem");
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

            res = curl_easy_perform(curl); //orden para la ejecucion
            if (res != 0) { //si falla
                printf("Error curl: %d\n", res); 
            }

            curl_easy_cleanup(curl); //libera el curl y cierra conexion
        } else {
            printf("No se pudo inicializar CURL\n");
        }

        if (chunk.response == NULL) {
            printf("No se recibio nada\n");
            sleep(2);
            continue;
        }

        // Si no hay mensajes nuevos
        if (strstr(chunk.response, "\"result\":[]") != NULL) {
            free(chunk.response);
            sleep(2);
            continue;
        }

        //punto 2
        char *p = chunk.response;
        char *ultimo_update = NULL;

        while ((p = strstr(p, "\"update_id\":")) != NULL) {
            ultimo_update = p;
            p++;  // avanzamos para seguir buscando
        }

        if (ultimo_update == NULL) {
            free(chunk.response);
            sleep(2);
            continue;
        }

        // Sacar update_id
        char *p_id = strchr(ultimo_update, ':');
        long update_id = -1;
        //pasa texto a numero
        if (p_id != NULL) {
            p_id++;  // despues de ':'
            while (*p_id == ' ' || *p_id == '\n') p_id++;
            update_id = strtol(p_id, NULL, 10);
        }
        
        //si el numero no es valido:
        if (update_id <= 0) {
            free(chunk.response);
            sleep(2);
            continue;
        }

        // Si ya lo procesamos, no hacemos nada
        if (update_id <= last_update_id) {
            free(chunk.response);
            sleep(2);
            continue;
        }

        last_update_id = update_id;

        //extraer la data importante del JSON

        // date
        long date = -1;
        char *p_date = strstr(ultimo_update, "\"date\":");
        if (p_date != NULL) {
            char *pd = strchr(p_date, ':');
            if (pd != NULL) {
                pd++;
                while (*pd == ' ' || *pd == '\n') pd++;
                date = strtol(pd, NULL, 10);
            }
        }

        // chat.id
        long chat_id = -1;
        char *p_chat = strstr(ultimo_update, "\"chat\"");
        if (p_chat != NULL) {
            char *p_cid = strstr(p_chat, "\"id\":");
            if (p_cid != NULL) {
                char *pc = strchr(p_cid, ':');
                if (pc != NULL) {
                    pc++;
                    while (*pc == ' ' || *pc == '\n') pc++;
                    chat_id = strtol(pc, NULL, 10);
                }
            }
        }

        // first_name
        char first_name[64] = "";
        char *p_name = strstr(ultimo_update, "\"first_name\":\"");
        if (p_name != NULL) {
            p_name += strlen("\"first_name\":\"");
            int i = 0;
            while (p_name[i] != '"' && p_name[i] != '\0'
                   && i < (int)sizeof(first_name) - 1) {
                first_name[i] = p_name[i];
                i++;
            }
            first_name[i] = '\0';
        }

        // text
        char text[256] = "";
        char *p_text = strstr(ultimo_update, "\"text\":\"");
        if (p_text != NULL) {
            p_text += strlen("\"text\":\"");
            int i = 0;
            while (p_text[i] != '"' && p_text[i] != '\0'
                   && i < (int)sizeof(text) - 1) {
                text[i] = p_text[i];
                i++;
            }
            text[i] = '\0';
        }

        printf("update_id: %ld\n", update_id);
        printf("date: %ld\n", date);
        printf("chat_id: %ld\n", chat_id);
        printf("first_name: %s\n", first_name);
        printf("text: %s\n", text);

        // escribir en el txt el historial de mensajes
        if (date != -1 && text[0] != '\0') {
            fprintf(log, "RECIBIDO %ld %s: %s\n", date, first_name, text);
            fflush(log);
        }

        // ver que  hay que responder
        if (chat_id != -1 && text[0] != '\0') {
            char lower[256];
            a_minusculas(text, lower, sizeof(lower));

            int es_hola = (strstr(lower, "hola") != NULL);
            int es_chau = (strstr(lower, "chau") != NULL);

            if (es_hola || es_chau) {
                char reply[256];

                if (es_hola) {
                    snprintf(reply, sizeof(reply),
                             "Hola %s", first_name);
                } else {
                    snprintf(reply, sizeof(reply),
                             "Chau %s", first_name);
                }

                // log de lo que escribe el bot
                if (date != -1) {
                    fprintf(log, "ENVIADO %ld BOT: %s\n", date, reply);
                    fflush(log);
                }

                // reemplazar espacios en el texto de respuesta
                char reply_encoded[512];
                encode_spaces(reply, reply_encoded, sizeof(reply_encoded));

                // armar URL de sendMessage 
                char send_url[512];
                snprintf(send_url, sizeof(send_url),
                         "%s/sendMessage?chat_id=%ld&text=%s",
                         base_url, chat_id, reply_encoded);

                // enviar mensaje
                CURL *curl2 = curl_easy_init();
                struct memory chunk2 = {0};
                if (curl2) {
                    curl_easy_setopt(curl2, CURLOPT_URL, send_url);
                    curl_easy_setopt(curl2, CURLOPT_WRITEFUNCTION, cb);
                    curl_easy_setopt(curl2, CURLOPT_WRITEDATA, (void *)&chunk2);
                    curl_easy_setopt(curl2, CURLOPT_CAINFO, "C:\\curl\\cacert.pem");
                    curl_easy_setopt(curl2, CURLOPT_SSL_VERIFYPEER, 0L);
                    curl_easy_setopt(curl2, CURLOPT_SSL_VERIFYHOST, 0L);

                    res = curl_easy_perform(curl2);
                    if (res != 0) {
                        printf("Error al enviar mensaje: %d\n", res);
                    }

                    free(chunk2.response);
                    curl_easy_cleanup(curl2);
                }
            }
        }

        free(chunk.response);
        sleep(2);  // cada 2 segundos 
    }

    fclose(log);


    return 0;
}
