function c = false_pos_zero(f, a, b, prec)
    max_it = 128;
    tol = 10 ^ (-prec - 1);

    for i = 1:max_it
        f_a = f(a);
        f_b = f(b);
        c = (b * f_a - a * f_b) / (f_a - f_b);
        f_c = f(c);

        if abs(f_c) < tol
            return
        end
        if f_a * f_c < 0
            b = c;
        else
            a = c;
        end
    end

    c = NaN
end