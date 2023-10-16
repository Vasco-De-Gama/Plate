#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../json-c/json.h"

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Создание дескриптора сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Привязка сокета к порту
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Ожидание подключения клиентов
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Принятие нового подключения
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Отправка данных каждую секунду
        while (1) {
            // Создание JSON объекта
            json_object *json_obj = json_object_new_object();
            json_object_object_add(json_obj, "TEMP", json_object_new_string("50"/*тут будет ссылка на нужное*/));
            json_object_object_add(json_obj, "CPU_FREQ", json_object_new_string("2000"));
            json_object_object_add(json_obj, "GPIO", json_object_new_string("asdasd"));


            // Преобразование JSON объекта в строку
            const char *json_str = json_object_to_json_string(json_obj);

            // Отправка данных клиенту
            send(new_socket, json_str, strlen(json_str), 0);
            printf("Data sent to client: %s\n");

            // Освобождение памяти
            json_object_put(json_obj);

            sleep(1);
        }
    }

    return 0;
}
