#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define SEQ_MAX 1024

int *highest_score(char *a, char *b, int limit_a, int limit_b, int *score_system);
char *reverse_string(char *s);
char **needleman(char *a, char *b, int *score_system);
char **hirschberg(char *a, char *b, int *score_system);
void print_solution(char **solution, char *a, char *b);
int find_partition (int *score_l, int *score_r, int length);
char *string_slice(char *a, int lower, int upper);
char **length_one_case(char *a, char *b);

/*
Strange bug when using gaattcagtta / ggatcga as input with the length 1 trivial solution
case. Could be a problem with the needleman algorithm. Could be an issue with the 
solutions being fed to the algorithm
*/

int main () {

	// input two sequences, for optimal performance, longest sequence first
	char a[SEQ_MAX], b[SEQ_MAX];
	printf("What is the first sequence\n");
	scanf("%s", a);
	printf("What is the second sequence\n");
	scanf("%s", b);

	/* array containing our score system, the 0 index is our match score,
	the 1 index is our mismatch score and the 2 index is our gap score
	*/
	int score_system[3] = {1,0,0};
	int len_a = strlen(a);
	int len_b = strlen(b);


	char **solution = malloc(sizeof(char*)*2);
	solution = hirschberg(a,b,score_system);
	print_solution(solution,a,b);
}


/* takes sequences a and b and returns a 2 dimension array with the 
	pointers to the Needleman-Wunsch aligned solution subsequence */
char **hirschberg(char *a, char *b, int *score_system) {
	// return NULL string if length of either string is 0
	int len_a = 0;
	if (a != NULL) {
		len_a = strlen(a);
	}

	int len_b = 0;
	if (b != NULL) {
		len_b = strlen(b);
	}

	// return trivial solution if either sequence is length 1 or 0
	if (len_a == 0) {
		printf("A length 0 EC\n");
		char **solution = malloc(sizeof(char*)*2);
		for (int i = 0; i < 2; i++) {
			solution[i] = malloc(sizeof(char)*len_b);
		}
		for (int i = 0; i < len_b; i++) {
			solution[0][i] = '-';
			solution[1][i] = b[i];
		}
		return solution;
	}
	else if (len_b == 0) {
		printf("B length 0 EC\n");
		char **solution = malloc(sizeof(char*)*2);
		for (int i = 0; i < 2; i++) {
			solution[i] = malloc(sizeof(char)*len_a);
		}
		for (int i = 0; i < len_a; i++) {
			solution[1][i] = '-';
			solution[0][i] = a[i];
		}
		return solution;
	}
	else if (len_a == 1 || len_b == 1) {
		printf("a: %s, b: %s\n",a, b);
		printf("Length 1 Edge Case\n");
		char **solution = malloc(sizeof(char*) * 2);
		solution = length_one_case(a,b);
		printf("%s\n",solution[0]);
		printf("%s\n",solution[1]);
		return solution;
	}

	char *rev_a = reverse_string(a);
	char *rev_b = reverse_string(b);
	int a_mid = len_a/2;
	int *score_l = highest_score(a, b, a_mid, len_b, score_system);
	int *score_r = highest_score(rev_a, rev_b, a_mid + 1, len_b, score_system);
	int partition = find_partition(score_l, score_r, len_b);

	/*
	printf("Score_L:\n");
	for (int i = 0; i < len_b + 1; i++) {
		printf("%d ", score_l[i]);
	}
	printf("\n");
	for (int i = 0; i < len_b + 1; i++) {
		printf("%d ", score_r[i]);
	}
	printf("\n");

	printf("Parition: %d\n", partition);
	*/

	char **solution = malloc(sizeof(char*)*2);

	//slice string and run hirchberg again 
	char *a_upper = string_slice(a,0,a_mid);
	char *a_lower = string_slice(a,a_mid,len_a);
	char *b_left = string_slice(b,0,partition);
	char *b_right = string_slice(b,partition,len_b);

	char **solution_l = malloc(sizeof(char*)*2);
	char **solution_r = malloc(sizeof(char*)*2);
	solution_l = hirschberg(a_upper,b_left,score_system);
	solution_r = hirschberg(a_lower,b_right,score_system);

	// concatenate solution_l and solution_r into single solution
	char *buffer_a = malloc(sizeof(char)*SEQ_MAX);
	if (solution_l[0] != NULL) {
		strcat(buffer_a,solution_l[0]);
	}
	if (solution_r[0] != NULL) {
		strcat(buffer_a,solution_r[0]);
	}
	solution[0] = buffer_a;

	char *buffer_b = malloc(sizeof(char)*SEQ_MAX);
	if (solution_l[1] != NULL) {
		strcat(buffer_b,solution_l[1]);
	}
	if (solution_r[1] != NULL) {
		strcat(buffer_b,solution_r[1]);
	}
	solution[1] = buffer_b;
	

	return solution;

}

char **length_one_case(char *a, char *b) {
	int len_a = strlen(a);
	int len_b = strlen(b);
	int length = len_b;
	bool a_size1 = true;
	if (len_b == 1) {
		a_size1 = false;
		length = len_a;
	}
	char **solution = malloc(sizeof(char*)*2);
	for (int i = 0; i < 2; i++) {
		solution[i] = malloc(sizeof(char)*length);
	}
	if (a_size1) {
		for (int i = 0; i < length; i++) {
			if (a[0] == b[i]) {
				solution[0][i] = a[0];
				solution[1][i] = b[i];
			}
			else {
				solution[0][i] = '-';
				solution[1][i] = b[i];
			}
		}
	}
	else {
		for (int i = 0; i < length; i++) {
			if (b[0] == a[i]) {
				solution[0][i] = a[i];
				solution[1][i] = b[0];
			}
			else {
				solution[0][i] = a[i];
				solution[1][i] = '-';
			}
		}
	}
	return solution;
}

// finds the partition that maximizes score 
int find_partition (int *score_l, int *score_r, int length) {
	int index = 0;
	int max = -256;
	for (int i = 0; i < length + 1; i++) {
		if (score_l[i] + score_r[length-i] >= max) {
			max = score_l[i] + score_r[length-i];
			index = i;
		}
	}
	return index;
}

/* slices string according to upper and lower bound provided
INCLUDES LOWER, BUT DOES NOT INCLUDE UPPER 
(includes index a[lower] to a[upper - 1])
*/
char *string_slice(char *a, int lower, int upper) {
	if (upper <= lower) {
		return NULL;
	}
	char *slice = malloc(sizeof(char)*SEQ_MAX);
	int ctr = 0;
	for (int i = lower; i < upper; i++) {
		slice[ctr] = a[i];
		ctr++;
	}
	printf("%s\n", slice);
	return slice;
}

/* takes string a and string b and integer array score_system and returns the 
an int array of the last row created 
*/
int *highest_score(char *a, char *b, int limit_a, int limit_b, int *score_system) {
	int match = score_system[0];
	int mismatch = score_system[1];
	int gap = score_system[2];

	int len_a = strlen(a);
	int len_b = strlen(b);

	// initialize matrix (a 2 x len_a + 1 array)
	int **matrix = malloc(sizeof(int*) * 2);
	for (int i = 0; i < 2; i++) {
		matrix[i] = malloc(sizeof(int) * len_b + 1);
	}

	// initialize first row and column of matrix to 0
	matrix[0][0] = 0;
	matrix[1][0] = gap;
	for (int i = 1; i < len_a + 1; i++) {
		matrix[0][i] = matrix[0][i-1] + gap;
	}



	for (int i = 1; i < limit_a + 1; i++) {
		for (int j = 1; j < len_b + 1; j++) {
			// match/mismatch score
			int match_score = 0;
			if (a[i - 1] == b[j - 1]) {
				match_score = matrix[0][j - 1] + match;
			}
			else {
				match_score = matrix[0][j - 1] + mismatch;
			}

			// find max of match/mismatch diagonal and neighbors
			int gap_up = matrix[0][j] + gap;
			int gap_left = matrix[1][j - 1] + gap;
			if (gap_up > match_score) {
				if (gap_up > gap_left) {
					matrix[1][j] = gap_up;
				}
				else {
					matrix[1][j] = gap_left;
				}
			}
			else {
				if (match_score > gap_left) {
					matrix[1][j] = match_score;
				}
				else {
					matrix[1][j] = gap_left;
				}
			}
		}
		for (int k = 0; k < len_b + 1; k++) {
			matrix[0][k] = matrix[1][k];
			matrix[1][k] = 0;
		}
	}
	for (int i = 0; i < len_b + 1; i++) {
		printf("%d ",matrix[0][i]);
	}
	printf("\n");
	return matrix[0];

}

// reverses string
char *reverse_string(char *s) {
	int len = strlen(s);
	char *output = malloc(sizeof(char) * len);
	for (int i = 0; i < len; i++) {
		output[len - i - 1] = s[i];
	}
	return output;
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
		for (int j = 0; j < length + 2; j++) {
			printf("%c", solution[i][j]);
		}
		printf("\n");
	}
	return;
}
