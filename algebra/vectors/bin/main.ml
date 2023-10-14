let rec read_input () =
  let line = try Some (read_line ()) with End_of_file -> None in
  match line with Some text -> text ^ "\n" ^ read_input () | None -> ""

let ( >> ) f g x = g (f x)
let flip f a b = f b a

let vec_re =
  {|\(((?:-?\d+(?:\.\d*)?(?: |,|, ))*-?\d+(?:\.\d*)?)\)|}
  |> Re.Pcre.re ~flags:[ `MULTILINE ]
  |> Re.compile

let sep_re = {|( |,|, )|} |> Re.Pcre.re |> Re.compile

let parse_input s =
  Re.all vec_re s
  |> List.map
       (flip Re.Group.get 1 >> Re.split sep_re >> List.map Q.of_string
      >> Array.of_list)
  |> Array.of_list

let get_print_width m col =
  let w = ref 0 in
  for i = 0 to Array.length m - 1 do
    let len = String.length (Q.to_string m.(i).(col)) in
    if len > !w then w := len
  done;
  !w + 1
;;

match read_input () |> parse_input |> Vec.are_linear_dependent with
| Error _ -> Printf.eprintf "Vectorii trebuie sa aiba acelasi ordin\n"
| Ok None -> Printf.printf "Vectorii sunt liniar independenti\n"
| Ok (Some eq_matrix) ->
    Printf.printf "Vectorii sunt liniar dependenti; matricea ecuatiei este:\n";
    let rows = Array.length eq_matrix in
    let cols = Array.length eq_matrix.(0) in
    for i = 0 to rows - 1 do
      for j = 0 to cols + 1 do
        match (i, j) with
        | 0, 0 -> print_string "┌"
        | _, 0 when i < rows - 1 -> print_string "│"
        | _, 0 -> print_string "└"
        | i, j when j >= 1 && j <= cols ->
            Printf.printf "%*s"
              (get_print_width eq_matrix (j - 1))
              (Q.to_string eq_matrix.(i).(j - 1))
        | i, j when j = cols + 1 ->
            print_string
              (" │ 0 "
              ^ if i = 0 then "┐" else if i = rows - 1 then "┘" else "│")
        | _ -> failwith "unreachable"
      done;
      print_newline ()
    done
