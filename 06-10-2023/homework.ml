let combine_funcs op f g x = op (f x) (g x)

let sum_combine = combine_funcs ( + )
