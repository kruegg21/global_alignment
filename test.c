#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *highest_score(char *a, char *b, int limit_a, int limit_b, int *score_system);
int find_partition (int *score_l, int *score_r, int length);

int main () {
	
	char buffer[64];
	strcat(buffer, "cat");
	char *a = NULL;
	strcat(buffer, a);
	printf("%s",buffer);
}

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
	for (int i = 0; i < len_a + 1; i++) {
		matrix[0][i] = 0;
	}
	matrix[1][0] = 0;


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
	return matrix[0];
}
