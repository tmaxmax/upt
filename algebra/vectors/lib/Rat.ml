type t = { num : int; denom : int }

let of_int num = { num; denom = 1 }
let of_float (_ : float) : t = failwith "unimplemented"
let zero = of_int 0
let one = of_int 1

let norm v =
  let d = Ints.gcd v.num v.denom in
  { num = v.num / d; denom = v.denom / d }

exception Denominator_zero

let make num = function
  | 0 -> Error Denominator_zero
  | denom -> Ok (norm { num; denom })

let inv x = { num = x.denom; denom = x.num }
let eval r = float_of_int r.num /. float_of_int r.denom

let div a b =
  if b = zero then Error Division_by_zero
  else if b = one then Ok a
  else Ok (norm { num = a.num * b.denom; denom = a.denom * b.num })

module Ops = struct
  let ( /: ) num denom =
    match make num denom with Ok r -> r | Error e -> raise e

  let ( +/ ) a b =
    norm
      { num = (a.num * b.denom) + (b.num * a.denom); denom = a.denom * b.denom }

  let ( ~/ ) x = { num = -x.num; denom = x.denom }
  let ( -/ ) a b = a +/ ~/b
  let ( */ ) a b = norm { num = a.num * b.num; denom = a.denom * b.denom }
  let ( // ) a b = match div a b with Ok v -> v | Error e -> raise e
end
