function rob(nums: number[]): number {
  if (nums.length === 0) {
    return 0;
  }
  if (nums.length === 1) {
    return nums[0];
  }

  const memo = Array<number>(nums.length - 1).fill(0);
  memo[0] = 0;
  memo[1] = nums[0];
  for (let i = 1; i < nums.length - 1; i++) {
    memo[i + 1] = Math.max(memo[i], memo[i - 1] + nums[i]);
  }
  const res1 = memo[nums.length - 1];

  memo.fill(0);
  memo[nums.length - 1] = 0;
  memo[nums.length - 2] = nums[nums.length - 1];
  for (let i = nums.length - 2; i >= 0; i--) {
    memo[i - 1] = Math.max(memo[i], memo[i + 1] + nums[i]);
  }
  const res2 = memo[0];

  return Math.max(res1, res2);
}
