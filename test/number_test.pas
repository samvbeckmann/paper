PROGRAM number_test(input, output);
var num1, num2: integer;

function fibonacci(first, second, num: integer): integer;
BEGIN
    if num <= 0 then fibonacci := second
    else fibonacci := fibonacci(second, first + second, num - 1)
END;

function gcd(first, second: integer): integer;
begin
    if second = 0 then gcd := first
    else gcd := gcd(second, first mod second)
end;

function sum_below(first, second, max: integer): longInt;
var temp: integer; total: LongInt;
begin
    temp := 1;
    total := 0;
    while temp < max do
        begin
            if (temp mod first = 0) or (temp mod second = 0) then
                total := total + temp;
            temp := temp + 1
        end;
    sum_below := total
end;

BEGIN
    read(num1, num2);
    writeln(num1, 'th Fibonacci Number: ', fibonacci(0, 1, num1 - 1));
    writeln('Greatest Common Denominator: ',  gcd(num1, num2));
    write('Sum of multiples of ', num1, ' and ', num2, ' under 1k: ');
    writeln(sum_below(num1, num2, 1000));
END.
