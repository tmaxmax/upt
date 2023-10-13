open Rat.Ops

let%test _ = 5 /: -2 = -5 /: 2
let%test _ = -100 /: -20 = 5 /: 1
let%test _ = 57 /: -49 = -57 /: 49
let%test _ = Rat.of_float 4.24 |> Option.get |> Rat.eval = 4.24
