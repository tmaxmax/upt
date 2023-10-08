let rec pow a = function
  | 0 ->
      1
  | 1 ->
      a
  | n ->
      let b = pow a (n / 2) in
      b * b * if n mod 2 = 0 then 1 else a

let check_fermat a b c n =
  let res = pow a n + pow b n = pow c n in
  not (res && n != 2)

let is_triangle a b c = a + b >= c || a + c >= b || b + c >= a

let rec read_three_nats () =
  match
    try read_line () |> String.split_on_char ' ' |> List.map int_of_string
    with End_of_file -> [-1]
  with
  | [a; b; c] when a > 0 && b > 0 && c > 0 ->
      (a, b, c)
  | [-1] ->
      print_newline () ; exit 0
  | _ ->
      print_string "Invalid input! Try again: " ;
      read_three_nats ()

let rec read_nat () =
  try
    match read_int () with
    | n when n > 0 ->
        n
    | _ ->
        print_string "Invalid input! Try again: " ;
        read_nat ()
  with End_of_file -> print_newline () ; exit 0

let is_triangle_program () =
  print_string
    {|Can your sticks form a triangle?
Input your three sticks' lengths: |} ;
  let a, b, c = read_three_nats () in
  if is_triangle a b c then print_string "Yup!\n"
  else print_string "No, I'm sorry...\n"

let check_fermat_program () =
  print_string {|Is Fermat wrong?
Please input three integers: |} ;
  let a, b, c = read_three_nats () in
  print_string "Input exponent: " ;
  let n = read_nat () in
  Printf.printf "%d + %d = %d?\n" (pow a n) (pow b n) (pow c n) ;
  if check_fermat a b c n then print_string "...No, that doesn't work.\n"
  else print_string "...Holy smokes, Fermat was wrong!\n"

let main = function
  | [|_; "fermat"|] ->
      check_fermat_program ()
  | [|_; "triangle"|] ->
      is_triangle_program ()
  | _ ->
      print_string "No program selected!\n" ;
      exit 1
;;

main Sys.argv
