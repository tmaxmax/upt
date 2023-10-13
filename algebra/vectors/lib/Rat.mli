type t

val of_int : int -> t
val of_float : float -> t
val zero : t
val one : t

exception Denominator_zero

val make : int -> int -> (t, exn) result
val inv : t -> t
val div : t -> t -> (t, exn) result
val eval : t -> float

module Ops : sig
  val ( /: ) : int -> int -> t
  val ( +/ ) : t -> t -> t
  val ( ~/ ) : t -> t
  val ( -/ ) : t -> t -> t
  val ( */ ) : t -> t -> t
  val ( // ) : t -> t -> t
end
