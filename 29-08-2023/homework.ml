let interval a b c = a <= c && c <= b

let sort a b c =
  let first, last = (min a (min b c), max a (max b c)) in
  (first, a + b + c - first - last, last)

let timp_in_secunde ts =
  let h = int_of_string (String.sub ts 0 2) in
  let m = int_of_string (String.sub ts 5 2) in
  let s = int_of_string (String.sub ts 10 2) in
  (h * 60 * 60) + (m * 60) + s

let diferenta_timp a b = abs (timp_in_secunde a - timp_in_secunde b)

let arie_lungime_cerc r = (2. *. Float.pi *. r, Float.pi *. Float.pow r 2.)

let rec numar_zile_dintre_ani a b =
  if a = b then 0
  else if a > b then numar_zile_dintre_ani b a
  else
    let nr_ani = b - a in
    let nr_ani_divizibili_cu m =
      (* primul an din interval div. cu m *)
      let prim = if a mod m = 0 then a else a + (m - (a mod m)) in
      (* adunam 1 daca nr. nu e div. pt. ca altfel am pierde un an.
         De exemplu, in intervalul [2, 9) sunt 7 nr. si 2 div. cu 4
         insa 7 / 4 = 1. *)
      if prim < b then ((b - prim) / m) + if (b - prim) mod m == 0 then 0 else 1
      else 0
    in
    (* Pentru a calcula anii bisecti, numaram anii divizibili cu 4,
       scadem din acesta numarul de ani divizibili cu 100 si in final
       adunam inapoi numarul de ani divizibili cu 400. *)
    (365 * nr_ani) + nr_ani_divizibili_cu 4 - nr_ani_divizibili_cu 100
    + nr_ani_divizibili_cu 400

let main () =
  Printf.printf "%d = 731\n" (numar_zile_dintre_ani 2023 2025) ;
  Printf.printf "%d = 9862\n" (numar_zile_dintre_ani 2023 2050) ;
  Printf.printf "%d = 138792\n" (numar_zile_dintre_ani 2021 2401) ;
  Printf.printf "%d = 1094632\n" (numar_zile_dintre_ani 3 3000)
;;

main ()
