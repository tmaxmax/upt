function x = gauss_seidel(A, x0, b, prec)
    n = length(A);
    D = diag(A);

    function x = step(xk)
        x = xk;
        for i = 1:n
            x(i) = b(i); 
            for j = 1:n
                if i != j
                    x(i) = x(i) - A(i, j) * x(j);
                end
            end
            x(i) = x(i) / D(i);
        end
    end

    x = fpi_zero(@step, x0, prec);
end