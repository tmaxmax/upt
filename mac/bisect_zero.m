function c = bisect_zero (f, a, b, prec)
    n = ceil((prec + log10(b - a)) / log10(2));
    f_c = 0;

    for i = 1:n
        c = (a + b) / 2;
        f_c = f(c);

        if f_c == 0
            break;
        end
        if f(a) * f_c < 0
            b = c;
        else
            a = c;
        end
    end

    tol = 10 ^ prec;
    tol_inv = 1 / tol;

    if abs(round(f_c * tol) * tol_inv) > tol_inv
        c = NaN;
    end
end