impl Solution {
    pub fn most_points(questions: Vec<Vec<i32>>) -> i64 {
        let mut memo = vec![0i64; questions.len() + 1];
        for i in (0..questions.len()).rev() {
            let points = questions[i][0];
            let brainpower = questions[i][1];
            let j = i + brainpower as usize + 1;
            memo[i] = std::cmp::max(
                memo[i + 1],
                points as i64 + (j < questions.len()).then(|| memo[j]).unwrap_or(0),
            );
        }
        memo[0]
    }
}
