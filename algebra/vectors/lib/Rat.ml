type t = { num : int; denom : int }

let of_int num = { num; denom = 1 }
let zero = of_int 0
let one = of_int 1

let norm v =
  let d = Ints.gcd v.num v.denom in
  { num = v.num / d; denom = v.denom / d }

let decode_float f =
  let open Int64 in
  let bits = bits_of_float f in
  let sign = if shift_right bits 63 = 0L then 1 else -1 in
  let exp = logand (shift_right bits 52) 0x7ffL in
  let mant =
    let masked_bits = logand bits 0xfffffffffffffL in
    if exp = 0L then shift_left masked_bits 1
    else logor masked_bits 0x10000000000000L
  in
  let exp = sub exp (add 1023L 52L) in
  (mant, exp, sign)

let of_float f =
  if not (Float.is_finite f) then None
  else
    let m, e, s = decode_float f in
    if e < 0L then
      let open Ratio in
      let open Big_int in
      let denom = shift_left_big_int (big_int_of_int 1) (-Int64.to_int e) in
      let num = mult_int_big_int s (big_int_of_int (Int64.to_int m)) in
      let rat = create_normalized_ratio num denom in
      Some
        (norm
           {
             num = int_of_big_int (numerator_ratio rat);
             denom = int_of_big_int (denominator_ratio rat);
           })
    else
      let num = Int64.shift_left m (Int64.to_int e) in
      Some { num = Int64.to_int num; denom = 1 }

let make num = function 0 -> None | denom -> Some (norm { num; denom })
let inv x = { num = x.denom; denom = x.num }
let eval r = float_of_int r.num /. float_of_int r.denom

let div a b =
  if b = zero then None
  else if b = one then Some a
  else Some (norm { num = a.num * b.denom; denom = a.denom * b.num })

exception Denominator_zero

let print oc r = Printf.fprintf oc "%d/%d" r.num r.denom

module Ops = struct
  let ( /: ) num denom =
    match make num denom with Some r -> r | None -> raise Denominator_zero

  let ( +/ ) a b =
    norm
      { num = (a.num * b.denom) + (b.num * a.denom); denom = a.denom * b.denom }

  let ( ~/ ) x = { num = -x.num; denom = x.denom }
  let ( -/ ) a b = a +/ ~/b
  let ( */ ) a b = norm { num = a.num * b.num; denom = a.denom * b.denom }

  let ( // ) a b =
    match div a b with Some v -> v | None -> raise Division_by_zero
end
