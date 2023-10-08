let rec display_reversed s =
  if s = "" then ()
  else (
    display_reversed (String.sub s 1 (String.length s - 1)) ;
    print_char s.[0] ;
    print_newline () )
;;

display_reversed "sarmale"

let prefixes = "JKLMNOPQ"

let suffix = "ack"

let rec ducklings prefixes suffix =
  let prefix = String.sub prefixes 0 1 in
  let prefix = prefix ^ if prefix = "O" || prefix = "Q" then "u" else "" in
  print_string (prefix ^ suffix ^ "\n") ;
  let len = String.length prefixes in
  if len == 1 then () else ducklings (String.sub prefixes 1 (len - 1)) suffix
;;

ducklings prefixes suffix

let rec find' ctr word letter =
  if word.[0] = letter then ctr
  else
    let len = String.length word in
    if len = 1 then -1 else find' (ctr + 1) (String.sub word 1 (len - 1)) letter

let find = find' 0

let find_last word letter =
  let rec finder ctr =
    if ctr = -1 || word.[ctr] = letter then ctr else finder (ctr - 1)
  in
  finder (String.length word - 1)

let find_from word letter start =
  let len = String.length word in
  let rec finder ctr =
    if ctr = len then -1
    else if word.[ctr] = letter then ctr
    else finder (ctr + 1)
  in
  finder start

let count word letter =
  let len = String.length word in
  let rec counter i res =
    if i = len then res
    else if word.[i] = letter then counter (i + 1) (res + 1)
    else counter (i + 1) res
  in
  counter 0 0

let rotate_letter n ch =
  let code = Char.code ch in
  let first_ch =
    if 65 <= code && code <= 90 then 'A'
    else if 97 <= code && code <= 122 then 'a'
    else '\000'
  in
  if first_ch = '\000' then ch
  else
    let first_code = Char.code first_ch in
    let delta = (code - first_code + n) mod 26 in
    Char.chr (first_code + if delta < 0 then 26 + delta else delta)

let rotate_word n s = String.map (rotate_letter n) s ;;

print_string (rotate_word (-36) "melon" ^ "\n")
