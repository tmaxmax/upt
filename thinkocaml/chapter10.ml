let rec split_char_seq ch s () =
  try
    let open String in
    let i = index s ch in
    let part = sub s 0 i in
    let rest = sub s (i + 1) (length s - i - 1) in
    Seq.Cons (part, split_char_seq ch rest)
  with Not_found -> (
    match s with "" -> Seq.Nil | v -> Seq.Cons (v, Seq.empty) )

let write_sum in_file out_file =
  let cin = open_in in_file in
  let cout = open_out out_file in
  input_line cin |> split_char_seq ' ' |> Seq.flat_map String.to_seq
  |> Seq.map (fun a -> int_of_char a - int_of_char '0')
  |> Seq.fold_left ( + ) 0 |> string_of_int |> output_string cout ;
  close_out cout ;
  close_in cin

let combine_funcs f g op x = op (f x) (g x) ;;

write_sum "test.in" "test.out"
