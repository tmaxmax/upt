function x = secant_zero(f, x0, x1, prec)
    function x = fn(xi)
        f_xi = f(xi);
        x = xi - (f_xi * (xi - x0)) / (f_xi - f(x0));
        x0 = xi;
    end

    x = fpi_zero(@fn, x1, prec);
end