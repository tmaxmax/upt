function x = conj_gradients(A, x0, b)
    tmp = b - A * x0;
    d = tmp;
    r = tmp;
    n = length(A);
    x = x0;

    for k = 1:n
        if r == 0
            break;
        end

        a = (r' * r) / (d' * A * d);
        x = x + a * d;
        r_prev = r;
        r = r - a * A * d;
        bt = (r' * r) / (r_prev' * r_prev);
        d = r + bt * d;
    end
end