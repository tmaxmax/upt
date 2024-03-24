function x = newton_zero(f, df, x0, prec)
    x = fpi_zero(@(x) x - f(x) / df(x), x0, prec);
end