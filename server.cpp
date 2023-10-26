#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

int main() {
    int server, new_socket;
    struct sockaddr_in addr = {0};
    char buffer[256];
    char freq[10];

    //create socket
    if ((server = socket(AF_INET, SOCK_STREAM, 0)) ==0) {
        perror("Socket fail");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8888);

    //SockConnect to port
    if (bind(server, (struct sockaddr *) &addr, sizeof addr) < 0){
        perror("SockConnect fail");
        exit(EXIT_FAILURE);
    }
    //connect waiting
    if (listen(server, 3) < 0) {
        perror("coonect waiting fail");
        exit(EXIT_FAILURE);
    }

    while (1) {
        //accept connect and create new socket for them
        if (new_socket = accept(server, NULL, NULL) <0) {
            perror("accept fail");
            exit(EXIT_FAILURE);
        }

        FILE* file = fopen("/sys/devices/system/cpu/cpufreq/policy0/cpuinfo_cur_freq", "r");
        if (file == NULL) {
            perror ("Не удалось открыть файл\n");
            return 1;
        }

        if (fgets(freq, sizeof(freq), file) != NULL) {
            printf( "Текущая частота CPU: %\n", freq);
        } else {
            perror("Не удалось прочитать частоту CPU\n");
        }

        fclose(file);
        /* CPU_Cur_Freq, Cur_Temp, GPIO
       
        QJsonObject jsonTempObject;
        
        jsonTempObject["currentTemp"] = QjsonValue(currentTemp);

        Qstring jsonStringTemp = jsonTempObject.toJson();

        send json data
        */
        sleep(1);
    }

    return 0;

}
