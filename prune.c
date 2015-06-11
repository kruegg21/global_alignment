#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int needleman (char **solution, char *a, char *b, int *score_system);
void print_solution(char **solution, char *a, char *b);

int main () {
	char a[64], b[64];
	int c;
	printf("What is the first sequence\n");
	scanf("%s", a);
	printf("What is the second sequence\n");
	scanf("%s", b);
	printf("What is your minimum score guess\n");
	scanf("%d", &c);

	/* score_system contains the values for match, mismatch, gap and our score 
	minimum used for pruning. If a row in the table cannot possibly reach our
	score minimum, -1024 is inserted to indicate that we will prune the rest 
	of the row
	*/
	int scoresystem[4] = {1,-1,-1};
	scoresystem[3] = c;
	int path_len = 0;
	if (strlen(a) > strlen(b)) {
		path_len = strlen(a);
	}
	else {
		path_len = strlen(b);
	}
	char **solution = malloc(sizeof(char*)*2);
	for (int i = 0; i < 2; i++) {
		solution[i] = malloc(sizeof(char)*path_len + 10);
	}
	int err_status = needleman(solution, a, b, scoresystem);
	if (err_status == 1) {
		printf("FAILURE, PICK HIGHER MIN-SCORE");
	}
	else {
		print_solution(solution,a,b);
	}
}


/* estimates a good starting guess for our minimum_score guess. */
int estimate_min_score (char *a, char *b, int *score_system) {
	int match = score_system[0];
	int gap = score_system[2];

	int len_a = strlen(a);
	int len_b = strlen(b);
	int guess = 0;
	if (len_a < len_b) {
		// guess if now theoretical maximum for score
		guess = len_a * match + (len_b - len_a) * gap;
	}
	else {
		guess = len_a * match + (len_a - len_a) * gap;
	}
	return 0;

}

void print_solution(char **solution, char *a, char *b) {
	int length = 0;
	if (strlen(b) > strlen(a)) {
		length = strlen(b);
	}
	else {
		length = strlen(a);
	}

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < length; j++) {
			printf("%c", solution[i][j]);
		}
		printf("\n");
	}
	return;
}

/* returns 0 if we have reached a legitimate answer, and 1 if our guess has 
resulted in a failure
*/
int needleman (char **solution, char *a, char *b, int *score_system) {
	// match, mismatch, gap scores
	int match = score_system[0];
	int mismatch = score_system[1];
	int gap = score_system[2];
	int score_min = score_system[3];

	// length of our two strings
	int len_a = strlen(a);
	int len_b = strlen(b);
	bool a_longer = true;
	if (len_b > len_a) {
		a_longer = false;
	}

	// create matrix
	int **matrix = malloc(sizeof(int*) * (len_a + 1));
	for (int i = 0; i < len_a + 1; i++){
		matrix[i] = malloc(sizeof(int) * (len_b + 1));
	}


	// initialize first column and row to all gaps
	matrix[0][0] = 0;
	for (int i = 1; i < len_a + 1; i++) {
		matrix[i][0] = matrix[i-1][0] + gap;
	}

	for (int j = 1; j < len_b + 1; j++) {
		matrix[0][j] = matrix[0][j-1] + gap;
	}

	// run through matrix
	for (int i = 1; i < len_a + 1; i++) {
		for (int j = 1; j < len_b + 1; j++) {
			// check if we can prune
			if ((len_b + 1 - j)*match + matrix[i - 1][j - 1] < score_min) {
				printf("PRUNED\n");
				matrix[i][j] = -1024;
				j = len_b + 1;
			}
			else {
				// match/mismatch score
				int match_score = 0;
				if (a[i - 1] == b[j - 1]) {
					match_score = matrix[i - 1][j - 1] + match;
				}
				else {
					match_score = matrix[i - 1][j - 1] + mismatch;
				}

				// find max of match/mismatch diagonal and neighbors
				int gap_up = matrix[i - 1][j] + gap;
				int gap_left = matrix[i][j - 1] + gap;
				if (gap_up > match_score) {
					if (gap_up > gap_left) {
						matrix[i][j] = gap_up;
					}
					else {
						matrix[i][j] = gap_left;
					}
				}
				else {
					if (match_score > gap_left) {
						matrix[i][j] = match_score;
					}
					else {
						matrix[i][j] = gap_left;
					}
				}
			}
		}
	}

	// print matrix TEST CODE
	for (int i = 0; i < len_a + 1; i++) {
		for (int j = 0; j < len_b + 1; j++) {
			printf("%d ",matrix[i][j]);
		}
		printf("\n");
	}

	int a_pos = len_a;
	int b_pos = len_b;
	int path_len = len_b;
	if (a_longer) {
		path_len = len_a;
	}
	
	int path_ctr = 0;
	while ((a_pos > 0 && b_pos > 0) && path_ctr < path_len) {
		int temp = 0;
		// check if pruning has cut into our path ****PRUNING CHANGE****
		if (matrix[a_pos-1][b_pos-1] == -1024 || 
			matrix[a_pos-1][b_pos] == -1024) {
			return 1;
		}

		if (a[a_pos - 1] == b[b_pos - 1]) {
			temp = match;
		}
		else {
			temp = mismatch;
		}

		if (matrix[a_pos][b_pos] == matrix[a_pos-1][b_pos-1]+temp) 
		{
			solution[0][path_len - path_ctr - 1] = a[a_pos - 1];
			solution[1][path_len - path_ctr - 1] = b[b_pos - 1];
			a_pos--;
			b_pos--;
			path_ctr++;
		}
		else if (matrix[a_pos - 1][b_pos] == matrix[a_pos][b_pos] - gap) {
			solution[0][path_len - path_ctr - 1] = a[a_pos - 1];
			solution[1][path_len - path_ctr - 1] = '-';
			a_pos--;
			path_ctr++;
		}
		else {
			solution[0][path_len - path_ctr - 1] = '-';
			solution[1][path_len - path_ctr - 1] = b[b_pos - 1];
			b_pos--;
			path_ctr++;
		}
	}
	

	// free matrix
	for (int i = 0; i < len_a + 1; i++){
		free(matrix[i]);
	}
	free(matrix);

	return 0;
}
