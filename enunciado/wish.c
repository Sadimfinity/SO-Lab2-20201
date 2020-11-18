#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    char **mypath;
    char *bin = "/bin/";
    mypath = (char **)malloc(3 * sizeof(char *));
    mypath[0] = bin;
    mypath[1] = NULL;

    char error_message[30] = "An error has occurred\n";

    
    if (argc == 1) //si el argc es 1 entonces inicia el modo Interactivo
    {
        while (1)
        {
            printf("prompt> "); //Aqui se imprime el prompt para esperar la entrada del usuario
            char *line = NULL;
            size_t len = 0;
            ssize_t read;
            if ((read = getline(&line, &len, stdin)) == -1) //Se hace la lectura de la entrada por linea
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                continue;
            }
            line[strlen(line) - 1] = '\0';
            char *extra = strdup(line);
            char *var_pal_Aux;
            char *var_pal_Aux_2;
            char *var_pal_Aux_3;
            int verificacion = 0;

            for (int j = 0; (var_pal_Aux = strsep(&extra, " ")) != NULL; j++)
            {
                for (int k = 0; (var_pal_Aux_2 = strsep(&var_pal_Aux, "\t")) != NULL; k++)
                {
                    for (int k = 0; (var_pal_Aux_3 = strsep(&var_pal_Aux_2, "&")) != NULL; k++)
                    {
                        if (strcmp(var_pal_Aux_3, "") != 0)
                        {
                            verificacion = 1;
                        }
                    }
                }
            }
            if (verificacion == 0)
            {
                continue;
            }

            //Esta seccion de codigo es para guardar en una matriz cada comando
            char *palabra;
            char *matriz[100];
            for (int i = 0; (palabra = strsep(&line, "&")) != NULL; i++)
            {
                if (strcmp(palabra, "") != 0 && strcmp(palabra, "\t") != 0)
                {
                    matriz[i] = palabra;
                    matriz[i + 1] = NULL;
                }
                else
                {
                    i = i - 1;
                };
            };

            //Se hace un ciclo para cada recorrer la matriz de los comandos
            for (int i = 0; matriz[i] != NULL; i++)
            {
                //Separar las palabras de cada comando
                char *palabra2;
                char *palabra3;
                char *matriz2[100];
                char *file = NULL;
                char *file2 = NULL;
                int isRedirection = 0;
                int error = 0;
                char *args = strdup(matriz[i]);
                for (int j = 0; (palabra2 = strsep(&args, ">")) != NULL; j++)
                {
                    if (j == 0)
                    {
                        if (strcmp(palabra2, "") != 0)
                        {
                            matriz[i] = palabra2;
                        }
                        else
                        {
                            error = 1;
                        }
                    }
                    else if (j == 1)
                    {
                        isRedirection = 1;
                        file = palabra2;
                        if (file == NULL)
                        {
                            error = 1;
                        }
                    }
                    else
                    {
                        error = 1;
                        break;
                    }
                }
                if (isRedirection == 1)
                {
                    if (file != NULL)
                    {
                        int escrito = 0;
                        char *palabra6;
                        char *palabra7;

                        for (int j = 0; (palabra6 = strsep(&file, " ")) != NULL; j++)
                        {
                            for (int k = 0; (palabra7 = strsep(&palabra6, "\t")) != NULL; k++)
                            {
                                if (strcmp(palabra7, "") != 0)
                                {

                                    if (escrito == 0)
                                    {

                                        escrito = 1;
                                        file2 = palabra7;
                                        j = j + 1;
                                    }
                                    else
                                    {
                                        error = 1;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }

                if (error == 1 || (file2 == NULL && isRedirection == 1))
                {
                    write(STDERR_FILENO, error_message, strlen(error_message));
                    break;
                }
                for (int j = 0; (palabra2 = strsep(&matriz[i], " ")) != NULL; j++) //Ciclo para quitar los espacios de la palabra
                {
                    for (int k = 0; (palabra3 = strsep(&palabra2, "\t")) != NULL; k++) //Ciclo para quitar las tabulaciones de la palabra
                    {
                        if (strcmp(palabra3, "") != 0) //Aqui se verifica que la palabra contenga caracteres
                        {
                            matriz2[j] = palabra3;
                            matriz2[j + 1] = NULL;
                            j = j + 1;
                        }
                    }
                    j = j - 1;
                };

                if (i == 0) //Validacion de los comandos, exit - cd - path
                {
                    if (strcmp(matriz2[0], "exit") == 0)
                    {
                        if (matriz2[1] == NULL)
                        {
                            exit(0);
                            break;
                        }
                        else
                        {
                            write(STDERR_FILENO, error_message, strlen(error_message));
                            break;
                        }
                    };
                    if (strcmp(matriz2[0], "cd") == 0)
                    {

                        if (chdir(matriz2[1]) == -1)
                        {
                            write(STDERR_FILENO, error_message, strlen(error_message));
                        }

                        break;
                    };
                    if (strcmp(matriz2[0], "path") == 0)
                    {
                        if (matriz2[1] == NULL)
                        {
                            mypath[0] = NULL;
                        }
                        free(mypath);
                        mypath = (char **)malloc(sizeof(char *));
                        for (int k = 1; matriz2[k] != NULL; k++)
                        {
                            char *palabra = strdup(matriz2[k]);
                            char pathAct[200] = "./";
                            char pathAct2[200] = "";
                            char *auxxx;
                            char *slash = "/";
                            if (palabra[0] == 47)
                            {
                                strcat(pathAct2, matriz2[k]);
                                strcat(pathAct2, slash);
                                auxxx = strdup(pathAct2);
                            }
                            else
                            {
                                strcat(pathAct, matriz2[k]);
                                strcat(pathAct, slash);
                                auxxx = strdup(pathAct);
                            }
                            mypath[k - 1] = strdup(auxxx);
                            mypath[k] = NULL;
                        }

                        break;
                    };
                };
                int a = 1;
                for (int j = 0; mypath[j] != NULL; j++) //En este ciclo se hace el recorrido de la ruta
                {
                    char *path = strdup(mypath[j]);
                    strcat(path, matriz2[0]);
                    if (access(path, F_OK) == 0)
                    {
                        if (isRedirection == 1)
                        {

                            a = 0;
                            if (fork() == 0)
                            {
                                close(STDOUT_FILENO);
                                open(file2, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
                                execvp(path, matriz2);
                                return (0);
                            }
                        }
                        else
                        {
                            a = 0;
                            if (fork() == 0)
                            {
                                execv(path, matriz2);
                                return (0);
                            };
                        }
                        wait(NULL);
                        break;
                    }
                };
                if (a == 1) //validacion del comando en la ruta
                {
                    write(STDERR_FILENO, error_message, strlen(error_message));
                }
            };
        };
    };

    if (argc == 2) //si el argc es 2 entonces inicia el modo Batch
    {      
        FILE *fp = fopen(argv[1], "r"); //Se le el archivo con la funcion fopen
        if (fp == NULL)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        };
        FILE *fp2 = fopen(argv[1], "r");

        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        while ((read = getline(&line, &len, fp)) != -1) //Ciclo para leer linea por linea el archivo
        {
            int forks[100];
            forks[0] = 777;
            if (strcmp(line, "") != 0)
            {
                line[strlen(line) - 1] = '\0';
                char *extra = strdup(line);
                char *var_pal_Aux;
                char *var_pal_Aux_2;
                char *var_pal_Aux_3;
                int verificacion = 0;
                for (int j = 0; (var_pal_Aux = strsep(&extra, " ")) != NULL; j++)
                {
                    for (int k = 0; (var_pal_Aux_2 = strsep(&var_pal_Aux, "\t")) != NULL; k++)
                    {
                        for (int k = 0; (var_pal_Aux_3 = strsep(&var_pal_Aux_2, "&")) != NULL; k++)
                        {
                            if (strcmp(var_pal_Aux_3, "") != 0)
                            {
                                verificacion = 1;
                            }
                        }
                    }
                }
                if (verificacion == 0)
                {
                    continue;
                }
                //Ciclo para guardar en una matriz cada comando
                char *palabra;
                char *matriz[100];
                for (int i = 0; (palabra = strsep(&line, "&")) != NULL; i++)
                {
                    if (strcmp(palabra, "") != 0 && strcmp(palabra, "\t") != 0)
                    {
                        matriz[i] = palabra;
                        matriz[i + 1] = NULL;
                    }
                    else
                    {
                        i = i - 1;
                    };
                };

                for (int i = 0; matriz[i] != NULL; i++) //Ciclo para recorrer la matriz de los comandos
                {
                    char *palabra2;
                    char *palabra3;
                    char *matriz2[100];
                    char *file = NULL;
                    char *file2 = NULL;
                    int isRedirection = 0;
                    int error = 0;
                    char *args = strdup(matriz[i]);
                    for (int j = 0; (palabra2 = strsep(&args, ">")) != NULL; j++)
                    {
                        if (j == 0)
                        {
                            if (strcmp(palabra2, "") != 0)  
                            {
                                matriz[i] = palabra2;
                            }
                            else
                            {
                                error = 1;
                            }
                        }
                        else if (j == 1)
                        {
                            isRedirection = 1;
                            file = palabra2;
                            if (file == NULL)
                            {
                                error = 1;
                            }
                        }
                        else
                        {
                            error = 1;
                            break;
                        }
                    }
                    if (isRedirection == 1)
                    {
                        if (file != NULL)
                        {
                            int escrito = 0;
                            char *palabra6;
                            char *palabra7;

                            for (int j = 0; (palabra6 = strsep(&file, " ")) != NULL; j++)
                            {
                                for (int k = 0; (palabra7 = strsep(&palabra6, "\t")) != NULL; k++)
                                {
                                    if (strcmp(palabra7, "") != 0)
                                    {

                                        if (escrito == 0)
                                        {

                                            escrito = 1;
                                            file2 = palabra7;
                                            j = j + 1;
                                        }
                                        else
                                        {
                                            error = 1;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if (error == 1 || (file2 == NULL && isRedirection == 1))
                    {
                        write(STDERR_FILENO, error_message, strlen(error_message));
                        break;
                    }

                    for (int j = 0; (palabra2 = strsep(&matriz[i], " ")) != NULL; j++)
                    {
                        for (int k = 0; (palabra3 = strsep(&palabra2, "\t")) != NULL; k++)
                        {
                            if (strcmp(palabra3, "") != 0)
                            {
                                matriz2[j] = palabra3;
                                matriz2[j + 1] = NULL;
                                j = j + 1;
                            }
                        }
                        j = j - 1;
                    };

                    //Validamcion para saber si es comando exit-cd-path
                    if (i == 0)
                    {
                        if (strcmp(matriz2[0], "exit") == 0)
                        {
                            if (matriz2[1] == NULL)
                            {
                                exit(0);
                                break;
                            }
                            else
                            {
                                write(STDERR_FILENO, error_message, strlen(error_message));
                                break;
                            }
                        };
                        if (strcmp(matriz2[0], "cd") == 0)
                        {
                            if (chdir(matriz2[1]) == -1)
                            {
                                write(STDERR_FILENO, error_message, strlen(error_message));
                            }

                            break;
                        };
                        if (strcmp(matriz2[0], "path") == 0)
                        {
                            if (matriz2[1] == NULL)
                            {
                                mypath[0] = NULL;
                            }
                            free(mypath);
                            mypath = (char **)malloc(sizeof(char *));

                            for (int k = 1; matriz2[k] != NULL; k++)
                            {
                                char *palabra = strdup(matriz2[k]);
                                char pathAct[200] = "./";
                                char pathAct2[200] = "";
                                char *auxxx;
                                char *slash = "/";
                                if (palabra[0] == 47)
                                {
                                    strcat(pathAct2, matriz2[k]);
                                    strcat(pathAct2, slash);
                                    auxxx = strdup(pathAct2);
                                }
                                else
                                {
                                    strcat(pathAct, matriz2[k]);
                                    strcat(pathAct, slash);
                                    auxxx = strdup(pathAct);
                                }
                                mypath[k - 1] = strdup(auxxx);
                                mypath[k] = NULL;
                            }
                            break;
                        };
                    };
                    int a = 1;
                    //Ciclo para recorrer la ruta del path
                    for (int j = 0; mypath[j] != NULL; j++)
                    {
                        char *path = strdup(mypath[j]);
                        strcat(path, matriz2[0]);
                        if (access(path, F_OK) == 0)
                        {
                            a = 0;
                            int rc = fork();
                            forks[i] = rc;
                            forks[i + 1] = 777;
                            if (rc == 0)
                            {
                                if (isRedirection == 1)
                                {

                                    close(STDOUT_FILENO);
                                    open(file2, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
                                    execvp(path, matriz2);
                                    exit(0);
                                }

                                else
                                {

                                    execv(path, matriz2);
                                    exit(0);
                                }
                            }
                            break;
                        }
                    };
                    if (a == 1)  //Validacion para el comando en la ruta
                    {
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    }
                };
            };
            int status;
            for (int f = 0; forks[f] != 777; f++)
            {
                waitpid(forks[f], &status, 0);
            }
            wait(NULL);
        };
        fclose(fp);
        fclose(fp2);
        exit(0);
    };
    write(STDERR_FILENO, error_message, strlen(error_message)); //Se imprime el mensaje de error definido al principio en caso de que el usuario mande algo erroneo.
    exit(1);
}