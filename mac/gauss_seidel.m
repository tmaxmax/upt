function x = gauss_seidel(A, x0, b, w, prec)
    if ~exist('w', 'var') || isempty(w)
        w = 1;
    end
    if ~exist('prec', 'var') || isempty(prec)
        prec = 6;
    end

    n = length(A);

    function x = step(xk)
        x = xk;
        for i = 1:n
            coef = b(i); 
            for j = 1:n
                if i != j
                    coef = coef - A(i, j) * x(j);
                end
            end
            coef = coef / A(i, i);
            x(i) = (1 - w) * x(i) + w * coef;
        end
    end

    x = fpi_zero(@step, x0, prec);
end