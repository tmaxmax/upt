let split_words s =
  let r = Str.regexp "[.,:;?!\"'\r\n\t ]+" in
  let hyph = Str.regexp "-+" in
  Str.split r s |> List.filter (fun s -> s != "" && not (Str.string_match hyph s 0))
;;

read_line () |> split_words |> List.iter (Printf.printf "'%s' ") ;
print_newline