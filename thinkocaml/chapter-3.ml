let rec right_justify len s =
  if len <= String.length s then s else right_justify len (" " ^ s)

let concat_space last first = first ^ " " ^ last

let concat_plus = concat_space "+"

let concat_minus = concat_space "-"

let repeat s times =
  let rec impl res s times =
    if times <= 0 then res else impl (res ^ s) s (times - 1)
  in
  impl "" s times

let append s sym times = s ^ repeat sym times

let create_grid cell_size num_cells_on_row =
  let num_chars_per_col_row = ((cell_size + 1) * num_cells_on_row) + 1 in
  let rec create_row index margin filler res =
    if index = num_chars_per_col_row then res ^ "\n"
    else if index mod (cell_size + 1) = 0 then
      res ^ margin |> create_row (index + 1) margin filler
    else res ^ filler |> create_row (index + 1) margin filler
  in
  let rec create index res =
    if index = num_chars_per_col_row then res
    else if index mod (cell_size + 1) = 0 then
      create_row 0 "+" "-" res |> create (index + 1)
    else create_row 0 "|" " " res |> create (index + 1)
  in
  create 0 ""

let main () =
  print_string (right_justify 70 "sarmale" ^ "\n") ;
  print_string (create_grid 4 4)
;;

main ()
