let square_root x =
  let rec step x last curr =
    if abs_float (last -. curr) < 1e-10 then curr
    else step x curr ((curr +. (x /. curr)) /. 2.)
  in
  step x x (if x >= 4. then x /. 2. else 1.)

let rec num_digits = function n when n < 10 -> 1 | n -> 1 + num_digits (n / 10)

let test_square_root upto =
  let single_val x =
    let ours = square_root x in
    let theirs = sqrt x in
    let delta = abs_float (ours -. theirs) in
    Printf.sprintf "%*.1f  %-12.10f  %-12.10f  %g\n"
      (num_digits upto + 2)
      x ours theirs delta
  in
  let rec build n res =
    if n > upto then res else build (n + 1) (res ^ single_val (float_of_int n))
  in
  let res = build 1 "" in
  String.sub res 0 (String.length res - 1)

let rec fact = function 0. -> 1. | 1. -> 1. | n -> n *. fact (n -. 1.)

let estimate_pi () =
  let step k =
    let four_k = 4. *. k in
    fact four_k
    *. (1103. +. (26390. *. k))
    /. ((fact k ** 4.) *. (396. ** four_k))
  in
  let rec sum k res =
    let term = step (float_of_int k) in
    let res = res +. term in
    if term < 1e-15 then res else sum (k + 1) res
  in
  let inv = 2. *. sqrt 2. /. 9801. *. sum 0 0. in
  inv ** -1.

let main = function
  | [|"sqrt"; n|] ->
      square_root (float_of_string n) |> print_float
  | [|"sqrt"; "table"; n|] ->
      test_square_root (int_of_string n) |> print_string
  | [|"pi"|] ->
      estimate_pi () |> print_float
  | [|"digits"; n|] ->
      num_digits (int_of_string n) |> print_int
  | _ ->
      exit 1
;;

main (Array.sub Sys.argv 1 (Array.length Sys.argv - 1)) ;
print_newline ()
