open Printf

let print_int_list = List.iter (printf "%d ")

let do_with_int_list fn elems =
  List.map int_of_string elems |> fn |> print_int_list

let string_of_bool = function true -> "yes" | false -> "no"

let list_product = List.fold_left ( * ) 1

let sort_int_list_asc = List.sort ( - )

let[@tail_mod_cons] rec cumulative_sum' last l =
  match l with
  | [] ->
      []
  | a :: rest ->
      let v = last + a in
      v :: cumulative_sum' v rest

let cumulative_sum = cumulative_sum' 0

let remove_nth n = List.filteri (fun i a -> i != n)

let[@tail_mod_cons] rec remove_one x l =
  match l with
  | [] ->
      []
  | a :: rest when a = x ->
      rest
  | a :: rest ->
      a :: remove_one x rest

let remove_all x = List.filter (fun a -> a != x)

let rec is_sorted ?prev l =
  match (l, prev) with
  | [], _ ->
      true
  | a :: _, Some p when p > a ->
      false
  | a :: rest, _ ->
      is_sorted ~prev:a rest

let[@tail_mod_cons] rec explode = function
  | "" ->
      []
  | s ->
      s.[0] :: explode String.(sub s 1 (length s - 1))

let rec implode = function
  | [] ->
      ""
  | c :: rest ->
      String.make 1 c ^ implode rest

exception Not_a_char

let char_of_string s = if String.length s > 1 then raise Not_a_char else s.[0]

let rec has_duplicates = function
  | [] ->
      false
  | a :: rest ->
      if List.mem a rest then true else has_duplicates rest

let[@tail_mod_cons] rec remove_duplicates = function
  | [] ->
      []
  | a :: rest when List.mem a rest ->
      remove_duplicates rest
  | a :: rest ->
      a :: remove_duplicates rest

module Birthday = struct
  type t = {day: int; month: int}

  let gen () = {day= Random.int 31; month= 1 + Random.int 12}

  let gen_list n = List.init n (fun _ -> gen ())
end

let compute_birthday_paradox num_students num_samples =
  let num_duplicates =
    List.init num_samples (fun _ -> Birthday.gen_list num_students)
    |> List.map (fun x -> if has_duplicates x then 1 else 0)
    |> List.fold_left ( + ) 0
  in
  float_of_int num_duplicates /. float_of_int num_samples

let rec bisect' currf currl =
  let ff, rf =
    match currf with [] -> (None, []) | a :: rest -> (Some a, rest)
  in
  let rl = match currl with [_] | [] -> [] | _ :: _ :: rest -> rest in
  match rl with
  | [] ->
      ([], ff, rf)
  | _ ->
      let nff, nf, nrf = bisect' rf rl in
      (Option.get ff :: nff, nf, nrf)

let bisect l = bisect' l l

let rec binary_exists elem l =
  let before, mid, after = bisect l in
  match mid with
  | Some x ->
      if elem = x then true
      else if elem < x then binary_exists elem before
      else binary_exists elem after
  | None ->
      false

let reversed s = explode s |> List.rev |> implode

let rec find_reverse_pairs = function
  | [] ->
      []
  | a :: rest ->
      let rev = reversed a in
      if List.mem rev rest then
        (if a < rev then (a, rev) else (rev, a)) :: find_reverse_pairs rest
      else find_reverse_pairs rest

let main = function
  | "csum" :: elems when List.length elems > 0 ->
      do_with_int_list cumulative_sum elems
  | "rmn" :: n :: elems ->
      do_with_int_list (remove_nth (int_of_string n)) elems
  | "rmo" :: o :: elems ->
      do_with_int_list (remove_one (int_of_string o)) elems
  | "rma" :: a :: elems ->
      do_with_int_list (remove_all (int_of_string a)) elems
  | "rmd" :: elems ->
      do_with_int_list remove_duplicates elems
  | "sorted" :: elems ->
      List.map int_of_string elems
      |> is_sorted |> string_of_bool |> print_string
  | "stc" :: [s] ->
      explode s |> List.iter (printf "%c ")
  | "cts" :: chars ->
      List.map char_of_string chars |> implode |> print_string
  | "bpx" :: [students; samples] ->
      Random.self_init () ;
      compute_birthday_paradox (int_of_string students) (int_of_string samples)
      *. 100.
      |> printf "%.2f%%"
  | "search" :: word :: words when is_sorted words ->
      binary_exists word words |> string_of_bool |> print_string
  | "revp" :: words ->
      find_reverse_pairs words |> remove_duplicates
      |> List.iter (fun (a, b) -> printf "%s-%s " a b)
  | "inlk" :: _ ->
      print_string "What the heck is this??"
  | _ ->
      exit 1
;;

main (Array.to_list Sys.argv |> List.tl) ;
print_newline ()
