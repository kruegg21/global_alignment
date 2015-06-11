// Requires addition of a value of score_minimum inputted by the user

int pruned_needleman(string s1, string s2, string * s1_aligned,
      string * s2_aligned) {
   int ** matrix;
   int score, x, y, s1_pos, s2_pos, match_score, gap_up, gap_left, temp;

   // create matrix
   x = s1.length() + 1;
   y = s2.length() + 1;
   matrix = (int **) malloc(sizeof(int *) * x);
   for (int i = 0; i < x; ++i) {
      matrix[i] = (int *) malloc(sizeof(int) * y);
   }

   // initialize matrix
   matrix[0][0] = 0;
   for (int i = 1; i < x; ++i) {
      matrix[i][0] = matrix[i - 1][0] + gap;
   }
   for (int i = 1; i < y; ++i) {
      matrix[0][i] = matrix[0][i - 1] + gap;
   }

   // run through matrix
   for (int i = 1; i < x; ++i) {
      for (int j = 1; j < y; ++j) {
         if ()
         else {
            // match/mismatch score
            if (1[i - 1] == s2[j - 1]) {
               match_score = matrix[i - 1][j - 1] + match;
            } else {
               match_score = matrix[i - 1][j - 1] + mismatch;
            }
            // find max of match/mismatch diagonal and neighbors
            gap_up = matrix[i - 1][j] + gap;
            gap_left = matrix[i][j - 1] + gap;
            matrix[i][j] = max(match_score, max(gap_up, gap_left));
         }
      }
   }

   s1_pos = s1.length();
   s2_pos = s2.length();
   score = matrix[s1_pos][s2_pos];

   // traverse matrix to find path
   while (s1_pos > 0 || s2_pos > 0) {
      if (s1[s1_pos - 1] == s2[s2_pos - 1]) {
         temp = match;
      } else {
         temp = mismatch;
      }

      if (s1_pos == 0) { // no more characters for s1
         *s1_aligned = GAP_CHAR + *s1_aligned;
         *s2_aligned = s2[s2_pos - 1] + *s2_aligned;
         s2_pos--;
      } else if (s2_pos == 0) { // no more characters for s2
         *s1_aligned = s1[s1_pos - 1] + *s1_aligned;
         *s2_aligned = GAP_CHAR + *s2_aligned;
         s1_pos--;
      } else if (matrix[s1_pos][s2_pos] == matrix[s1_pos-1][s2_pos-1] + temp) {
         *s1_aligned = s1[s1_pos - 1] + *s1_aligned;
         *s2_aligned = s2[s2_pos - 1] + *s2_aligned;
         s1_pos--;
         s2_pos--;
      } else if (matrix[s1_pos][s2_pos] == matrix[s1_pos][s2_pos-1] + gap) {
         *s1_aligned = GAP_CHAR + *s1_aligned;
         *s2_aligned = s2[s2_pos - 1] + *s2_aligned;
         s2_pos--;
      } else { // matrix[s1_pos][s2_pos] == matrix[s1_pos-1][s2_pos] + gap
         *s1_aligned = s1[s1_pos - 1] + *s1_aligned;
         *s2_aligned = GAP_CHAR + *s2_aligned;
         s1_pos--;
      }
   }

   // free matrix
   for (int i = 0; i < x; ++i){
      free(matrix[i]);
   }
   free(matrix);

   return score;
}