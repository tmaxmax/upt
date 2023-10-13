let () = Printf.printf "%a\n" Rat.print (Rat.of_float 5.6 |> Option.get)
