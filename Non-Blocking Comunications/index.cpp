// Comunicações sem bloqueio - exercício

void play_non_blocking_scenario() {
    MPI_Request request;
    MPI_Status status;
    int request_finished = 0;

    // Inicialização do buffer:
    for (int i = 0; i < buffer_count; ++i)
        buffer[i] = (rank == 0 ? i * 2 : 0);

    MPI_Barrier(MPI_COMM_WORLD);
    // Início do cronômetro
    double time = -MPI_Wtime(); // Este comando nos ajuda a medir o tempo. Veremos mais sobre isso posteriormente!

    ////////// Não deve modificar nada ANTES deste ponto //////////

    if (rank == 0) {
        sleep(3);

        // 1- Inicializar o envio não bloqueante para o processo 1
        MPI_Isend(buffer, buffer_count, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);

        double time_left = 6000.0;
        while (time_left > 0.0) {
            usleep(1000); // Trabalhamos por 1ms

            // 2- Testar se a solicitação foi concluída (somente se ainda não estiver concluída)
            MPI_Test(&request, &request_finished, &status);
            if (request_finished) {
                break;
            }

            // 1ms restante para trabalhar
            time_left -= 1000.0;
        }

        // 3- Se a solicitação ainda não estiver completa, aguarde aqui.
        MPI_Wait(&request, &status);

        // Modificando o buffer para a segunda etapa
        for (int i = 0; i < buffer_count; ++i)
            buffer[i] = -i;

        // 4- Preparar outra solicitação para o processo 1 com uma tag diferente
        MPI_Isend(buffer, buffer_count, MPI_INT, 1, 1, MPI_COMM_WORLD, &request);

        time_left = 3000.0;
        while (time_left > 0.0) {
            usleep(1000); // Trabalhamos por 1ms

            // 5- Testar se a solicitação foi concluída (somente se ainda não estiver concluída)
            MPI_Test(&request, &request_finished, &status);
            if (request_finished) {
                break;
            }

            // 1ms restante para trabalhar
            time_left -= 1000.0;
        }
        // 6- Aguarde a conclusão
        MPI_Wait(&request, &status);
    } else {
        // Trabalhe por 5 segundos
        sleep(5);

        // 7- Inicializar a recepção não bloqueante do processo 0
        MPI_Irecv(buffer, buffer_count, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);

        // 8- Aguarde aqui até que a solicitação seja concluída
        MPI_Wait(&request, &status);

        print_buffer();

        // Trabalhe por mais 3 segundos
        sleep(3);

        // 9- Inicializar outra recepção não bloqueante
        MPI_Irecv(buffer, buffer_count, MPI_INT, 0, 1, MPI_COMM_WORLD, &request);

        // 10- Aguarde até que seja concluída
        MPI_Wait(&request, &status);

        print_buffer();
    }
    ////////// Não deve modificar nada DEPOIS deste ponto //////////

    // Fim do cronômetro
    time += MPI_Wtime();

    // Esta linha nos dá o tempo máximo decorrido em cada processo.
    // Veremos sobre redução posteriormente!
    double final_time;
    MPI_Reduce(&time, &final_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
        std::cout << "Tempo total para o cenário não bloqueante: " << final_time << "s" << std::endl;
}
