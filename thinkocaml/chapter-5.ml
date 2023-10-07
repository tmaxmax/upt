let rec fib = function 0 -> 0 | 1 -> 1 | n -> fib (n - 1) + fib (n - 2)

exception Negative_numbers

let rec ack m n =
  match (m, n) with
  | 0, n ->
      n + 1
  | m, 0 when m > 0 ->
      ack (m - 1) 1
  | m, n when m > 0 && n > 0 ->
      ack (m - 1) (ack m (n - 1))
  | _, _ ->
      raise Negative_numbers

let head_ch w = w.[0]

let tail_ch w = w.[String.length w - 1]

let middle w = String.sub w 1 (String.length w - 2)

let rec is_palindrome w =
  match String.length w with
  | 0 | 1 ->
      true
  | _ ->
      head_ch w = tail_ch w && is_palindrome (middle w)

let rec is_power a b = a mod b == 0 && is_power (a / b) b

let rec gcd a b = if b = 0 then a else gcd b (a mod b)

let string_of_bool = function true -> "yes" | false -> "no"

let hfm n =
  let rec f = function 0 -> 1 | n -> n - m (f (n - 1))
  and m = function 0 -> 0 | n -> n - f (m (n - 1)) in
  (f n, m n)

let main = function
  | [|"fib"; n|] ->
      fib (int_of_string n) |> print_int
  | [|"ack"; a; b|] ->
      ack (int_of_string a) (int_of_string b) |> print_int
  | [|"pal"; w|] ->
      is_palindrome w |> string_of_bool |> print_string
  | [|"pow"; a; b|] ->
      is_power (int_of_string a) (int_of_string b)
      |> string_of_bool |> print_string
  | [|"gcd"; a; b|] ->
      gcd (int_of_string a) (int_of_string b) |> print_int
  | [|"hfm"; n|] ->
      let f, m = hfm (int_of_string n) in
      Printf.printf "F=%d M=%d" f m
  | _ ->
      exit 1
;;

main (Array.sub Sys.argv 1 (Array.length Sys.argv - 1)) ;
print_newline
