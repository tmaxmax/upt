package dp

func rob(nums []int) int {
	if len(nums) == 0 {
		return 0
	}

	memo := make([]int, len(nums)+1)
	memo[0] = 0
	memo[1] = nums[0]

	for i := 1; i < len(nums); i++ {
		memo[i+1] = max(memo[i], memo[i-1]+nums[i])
	}

	return memo[len(nums)]
}
