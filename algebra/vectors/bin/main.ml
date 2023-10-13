let () =
  read_line () |> String.split_on_char ' '
  |> List.filter_map Rat.of_string
  |> List.fold_left Rat.Ops.( +/ ) Rat.zero
  |> Printf.printf "%a\n" Rat.print
