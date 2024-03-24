function x = fpi_zero(f, x0, prec)
    max_it = 128;
    # By having an absolute error of an order of magnitude
    # less than the desired number of precise digits we
    # ensure that we actually get that much precision.
    tol = 10 ^ (-prec - 1); 

    x = x0;

    for i = 1:max_it
        xi = f(x);
        err = xi - x;
        x = xi;
        if abs(err) < tol
            return
        end
    end

    x = NaN
end