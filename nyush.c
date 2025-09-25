#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>



int main(void) {
	// job array initialization 
	int num_job = 0;
	pid_t jb_pid[100];
	char *jb_cmd[100];

	while (1) {
		char *input_line = NULL;
		size_t buffer_size = 0;
		ssize_t num_char;
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		// milestone 1   
		char *cwd = getcwd(NULL, 0);
		char *basename_dir = cwd; 
		for (int i = 0; cwd[i] != '\0'; i++) {
			if (cwd[i] == '/') {
				basename_dir = &cwd[i+1];
			}
		}
		if (cwd[0] =='/' && cwd[1] == '\0') {
			basename_dir = cwd;
		}
		fprintf(stdout, "[nyush %s]$ ", basename_dir);
		fflush(stdout);
		free(cwd);
		// if EOF we break
		num_char = getline(&input_line, &buffer_size, stdin);
		if (num_char == -1) {
			free(input_line);
			break;
		}
		// if empty we just reprint the prompt again 
		input_line[num_char - 1] = '\0';

		if (input_line[0] == '|') {
			fprintf(stderr, "Error: invalid command\n");
			free(input_line);
			continue;
		}

		if (num_char > 1 && input_line[num_char - 2] == '<') {
			fprintf(stderr, "Error: invalid command\n");
			free(input_line);
			continue;
		}
		if (input_line[0] == '\0') {
			free(input_line);
			continue;
		}
		char *copy_of_cmd = strdup(input_line);
		if (!strchr(input_line, '|')) {
			char *argv[100];
			int argc = 0;
			int index = 0;

			//parsing through the input commands char by char

			while (input_line[index] != '\0' && argc < 99) {
				argv[argc] = &input_line[index];
				argc++;

				while (input_line[index] != '\0' && input_line[index] != ' '){
					index++;
				}
				if (input_line[index] == ' ') {
					input_line[index] ='\0';
					index++;

				}
			}
			// last argv has to be NULL to end token array 
			argv[argc] = NULL;
			
			if (strcmp(argv[0], "exit") ==0){
				// exit cannot have any arguments 
				if (argc != 1) {
					fprintf(stderr, "Error: invalid command\n");
					free(copy_of_cmd);
					free(input_line);
					continue;
				}
				// cant exit while there are suspended jobs so reprompt 
				if (num_job > 0) {
					fprintf(stderr, "Error: there are suspended jobs\n");
					free(copy_of_cmd);
					free(input_line);
					continue;
				}
				// if error scenarios dont hit then we can actually exit 
				free(copy_of_cmd);
				free(input_line);
				break;			
			}
			else if (strcmp(argv[0], "cd")== 0) {
				// can only be 1 argument 
				if (argc != 2) {
					fprintf(stderr, "Error: invalid command\n");
					free(copy_of_cmd);
					free(input_line);
					continue;
				}
				// change directories and if it results in error we print error message 
				if (chdir(argv[1]) !=0) {
					fprintf(stderr, "Error: invalid directory\n");
				}
				free(copy_of_cmd);
				free(input_line);
				continue;
			}
			else if (strcmp(argv[0], "jobs") == 0) {
				//jobs cannot habe any arguments 
				if (argc != 1) {
					fprintf(stderr, "Error: invalid command\n");
					free(copy_of_cmd);
					free(input_line);
					continue;
				}
				// print the jobs w/ indices 
				for (int i = 0; i < num_job; i++) {
					fprintf(stdout, "[%d] %s\n", i+1, jb_cmd[i]);
				}
				free(copy_of_cmd);
				free(input_line);
				continue;
			}

			else if (strcmp(argv[0], "fg") ==0) {
				// can only have 1 argument 
				if (argc !=2) {
					fprintf(stderr, "Error: invalid command\n");
					free(copy_of_cmd);
					free(input_line);
					continue;
				}
 
				int i = atoi(argv[1]) - 1;
				if (i < 0 || i >= num_job ) {
					// make sure the index is in the job range 
					fprintf(stderr, "Error: invalid job\n");
					free(copy_of_cmd);
					free(input_line);
					continue;
				}
				pid_t job_pid = jb_pid[i];
				char *orig_cmd = jb_cmd[i];
				// remove the job by shifting 
				for (int ind = i; ind < num_job - 1; ind++) {
					jb_pid[ind] = jb_pid[ind + 1];
					jb_cmd[ind] = jb_cmd[ind + 1];
				} 
				num_job--;
				// resume process
				if (kill(job_pid, SIGCONT) < 0) {
					free(orig_cmd);
					free(copy_of_cmd);
					free(input_line);
					continue;
				}

				int status;
				waitpid(job_pid, &status, WUNTRACED);
				// if process is stopped then we need to restore in jobs list 
				if (WIFSTOPPED(status)) {
					if (num_job < 100) {
						jb_pid[num_job] = job_pid;
						jb_cmd[num_job] = orig_cmd;
						num_job++;
					} else {
						free(orig_cmd);
						} 
					} else {
					free(orig_cmd);
				}
				free(copy_of_cmd);
				free(input_line);
				continue;
			}

			//io redirection 

			char *input = NULL;
			char *output = NULL;
			int append = 0;

			char *clean_argv[100];
			int clean_index = 0;

			for (int i = 0; i < argc; i++) {
				if (strcmp(argv[i], ">") == 0) {
					i++;
					output = argv[i];
				}
				else if (strcmp(argv[i], ">>") == 0) {
					i++;
					output = argv[i];
					append = 1;
				}
				else if (strcmp(argv[i], "<") == 0) {
					i++;
					input = argv[i];
					}

				else {
					clean_argv[clean_index] = argv[i];
					clean_index++;
				}
			}
			clean_argv[clean_index] = NULL;

			
			pid_t pid = fork();
			if (pid < 0) {
				free(copy_of_cmd);
				free(input_line);
				continue;

			}
			if (pid == 0) {
				//child 
				signal(SIGINT, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				signal(SIGTSTP, SIG_DFL);
				if (input) {
					int file = open(input, O_RDONLY);
					if (file < 0){
						fprintf(stderr, "Error: invalid file\n");
						exit(1);

					dup2(file, STDIN_FILENO);
					close(file);
				}
				if (output) {
					int file;
					if (append) {
						// Geeks4Geeks explanation on I/O system calls 
						file = open(output, O_WRONLY|O_CREAT | O_APPEND);
					}
					else {
						file = open(output, O_WRONLY|O_CREAT | O_TRUNC);
					}
					dup2(file, STDOUT_FILENO);
					close(file);
				}

				if (strchr(clean_argv[0], '/')) {
					execv(clean_argv[0], clean_argv);
				} else {
					char path[1024];
					snprintf(path, 1024, "/usr/bin/%s", clean_argv[0]);
					execv(path, clean_argv);
				}

				fprintf(stderr, "Error: invalid program\n");
				exit(1);
			}
			else {
				int status;
				waitpid(pid, &status, WUNTRACED);

				if (WIFSTOPPED(status)) {
					if (num_job < 100) {
						jb_pid[num_job] = pid;
						jb_cmd[num_job] = copy_of_cmd;
						num_job++;
					} 
					else {
						free(copy_of_cmd);

					}
					}
					else {
						free(copy_of_cmd);
				}
			}
			free(input_line);
		}
		// now for the situation where pipes exist 
		else {

			char *pipe_segs[100];
			int pipe_count = 0;

			char *xptr;
			char *part = strtok_r(copy_of_cmd, "|", &xptr);

			while (part && pipe_count < 100) {
				pipe_segs[pipe_count] = part;
				pipe_count++;
				part = strtok_r(NULL, "|", &xptr);
			}
			char *argv[100][100];

			char *input = NULL;
			char *output = NULL;
			int append = 0; 
			
			for (int i = 0; i < pipe_count; i++) {
				char *tokens[100];
				int argc = 0;

				char *xptr2;
				// split subcomms by spaces 
				char *tok = strtok_r(pipe_segs[i], " ", &xptr2);
				while (tok && argc < 99) {
					tokens[argc] = tok;
					argc++;
					tok = strtok_r(NULL, " ", &xptr2);
				}
				tokens[argc] = NULL;

			
				int ind = 0; 
				for (int j = 0; j < argc; j++) {
					// if < in the first subcomm then the next token is the input
					if ((strcmp(tokens[j], "<") == 0) && (i == 0)) {
						if ((j + 1) < argc && input == NULL) {
							input = tokens[j + 1];
							j++;
						}
						
						else {
							fprintf(stderr, "Error: invalid command\n");
							pipe_count = 0;
							break;
						}
					}
					else if ((strcmp(tokens[j], ">>") == 0 || strcmp(tokens[j], ">") == 0) && (i == pipe_count - 1)) {
						if (output) {
							fprintf(stderr, "Error: invalid command\n");
							pipe_count = 0;
							break;

						}
						if (strcmp(tokens[j], ">>") == 0) {
							append = 1;
						}

						if ((j + 1) < argc) {
							output = tokens[j + 1];
							j++;
						} else {
							fprintf(stderr, "Error: invalid command\n");
							pipe_count = 0;
							break;
						}

					}
					else {
						argv[i][ind] = tokens[j];
						ind++;
					}
				}
				argv[i][ind] = NULL; 
				
			}

			//cant pipe built ins 
			for (int i = 0; i < pipe_count; i++) {
				if (!argv[i][0]){
					continue;
				}
				if (strcmp(argv[i][0], "cd") == 0) {
					pipe_count = 0;
					break;
				}
				if (strcmp(argv[i][0], "exit") == 0) {
					pipe_count = 0;
					break;
				}
				if (strcmp(argv[i][0], "jobs") == 0) {
					pipe_count = 0;
					break;
				}
				if (strcmp(argv[i][0], "fg") == 0) {
					pipe_count = 0;
					break;
				}


			}
			//actually making the pipes 
			int pipefiles[200];
			for (int i = 0; i < (pipe_count -1); i++) {
				if (pipe(&pipefiles[2*i]) < 0) {
					pipe_count = 0;
					break;
				}
			}

			pid_t pids[100];
			for (int i = 0; i < pipe_count; i++){
				pid_t pid = fork();
				if (pid < 0) {
					pipe_count = 0;
					break;
				}

				else if (pid == 0) {
					//child 
					signal(SIGINT, SIG_DFL);
					signal(SIGQUIT, SIG_DFL);
					signal(SIGTSTP, SIG_DFL);

					// if not first then we can read from pipe 
					if (i > 0) {
						dup2(pipefiles[2*(i-1)], STDIN_FILENO);
					} else {
						if (input) {
							int file = open(input, O_RDONLY);
							if (file < 0) {
								fprintf(stderr, "Error: invalid file");
								exit(1);
							}
							dup2(file, STDIN_FILENO);
							close(file);
						}

					}
					// if not last write to pipe
					if ( i < pipe_count- 1) {
						dup2(pipefiles[2*i + 1], STDOUT_FILENO);
					}
					else {
						if (output) {
							int file;
							if (append) {
								file = open(output, O_WRONLY | O_CREAT | O_APPEND);

							}
							else {
								file = open(output, O_WRONLY | O_CREAT | O_TRUNC);
							}
							if (file < 0) {
								fprintf(stderr, "Error: invalid file\n");
								exit(1);
							}
							dup2(file, STDOUT_FILENO);
							close(file);
						}
					}
					for (int i = 0; i < 2*(pipe_count - 1); i++) {
						close(pipefiles[i]);
					}
					if (strchr(argv[i][0], '/')) {
						execv(argv[i][0], argv[i]);
					}
					else {
						char path[1024];
						snprintf(path, sizeof(path), "/usr/bin/%s", argv[i][0]);
						execv(path, argv[i]);
					}
					fprintf(stderr, "Error: invalid program\n");
					exit(1);
				} else {
					pids[i] = pid;
				}
			}
			//close pipes
			for (int i = 0; i <(2*(pipe_count - 1)); i++) {
				close(pipefiles[i]);
			}
			
			for (int i = 0; i < pipe_count; i++) {
				int status;
				waitpid(pids[i], &status, WUNTRACED);
				if (WIFSTOPPED(status)) {
					if (num_job < 100) {
						jb_pid[num_job] = pids[i];
						jb_cmd[num_job] = strdup(argv[i][0]);
						num_job++;
					}
				}
			}

			free(copy_of_cmd);
			free(input_line);			
		}
	}
	return 0;
}