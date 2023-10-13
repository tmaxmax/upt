type t = { num : int; denom : int }

let of_int num = { num; denom = 1 }
let of_float (_ : float) : t = failwith "unimplemented"
let zero = of_int 0
let one = of_int 1

let norm v =
  let d = Ints.gcd v.num v.denom in
  { num = v.num / d; denom = v.denom / d }

let make num = function 0 -> None | denom -> Some (norm { num; denom })
let inv x = { num = x.denom; denom = x.num }
let eval r = float_of_int r.num /. float_of_int r.denom

let div a b =
  if b = zero then None
  else if b = one then Some a
  else Some (norm { num = a.num * b.denom; denom = a.denom * b.num })

exception Denominator_zero

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
