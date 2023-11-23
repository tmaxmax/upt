let transform_to_row_echelon m =
  let rows = Array.length m in
  let cols = Array.length m.(0) in
  let rec find_pivot row pivot_col res =
    if row = rows then res
    else if Q.(abs m.(row).(pivot_col) > abs m.(res).(pivot_col)) then
      find_pivot (row + 1) pivot_col row
    else find_pivot (row + 1) pivot_col res
  in
  let rec eliminate_row row pivot_row coef col =
    if col = cols then ()
    else (
      m.(row).(col) <- Q.(m.(row).(col) - (m.(pivot_row).(col) * coef));
      eliminate_row row pivot_row coef (col + 1))
  in
  let rec row_swap i j col =
    if col = cols then ()
    else
      let tmp = m.(i).(col) in
      m.(i).(col) <- m.(j).(col);
      m.(j).(col) <- tmp;
      row_swap i j (col + 1)
  in
  let rec eliminate row pivot_row pivot_col =
    if row = rows then ()
    else
      let coef = Q.(m.(row).(pivot_col) / m.(pivot_row).(pivot_col)) in
      m.(row).(pivot_col) <- Q.zero;
      eliminate_row row pivot_row coef (pivot_col + 1);
      eliminate (row + 1) pivot_row pivot_col
  in
  let rec loop pivot_row pivot_col =
    if pivot_row = rows || pivot_col = cols then 0
    else
      let pivot_val_row = find_pivot pivot_row pivot_col pivot_row in
      if Q.equal m.(pivot_val_row).(pivot_col) Q.zero then
        loop pivot_row (pivot_col + 1)
      else (
        row_swap pivot_row pivot_val_row 0;
        eliminate (pivot_row + 1) pivot_row pivot_col;
        1 + loop (pivot_row + 1) (pivot_col + 1))
  in
  loop 0 0

let are_linear_dependent l =
  if not Array.(for_all (fun v -> length v = length l.(0)) l) then
    Error (Failure "Vectors must have the same size")
  else
    let rows = Array.length l.(0) in
    let cols = Array.length l in
    let eq_matrix =
      Array.(init rows (fun i -> init cols (fun j -> l.(j).(i))))
    in
    let num_pivots = transform_to_row_echelon eq_matrix in
    if num_pivots = cols then Ok None else Ok (Some eq_matrix)
