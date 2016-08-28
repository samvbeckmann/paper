program number_test(input, output);
var num1, num2: integer;

function fibonacci(first, second, num: integer): integer;
begin
    if num <= 0 then fibonacci := second
    else fibonacci := fibonacci(second, first + second, num - 1)
end;

function gcd(first, second: integer): integer;
begin
    if second = 0 then gcd := first
    else gcd := gcd(second, first mod second)
end;

function sum_below(first, second, max: integer): integer;
var temp total: integer;
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

begin
    read(num1, num2);
    writeln(num1, fibonacci(0, 1, num1 - 1));
    writeln(gcd(num1, num2));
    write( num1, num2);
    writeln(sum_below(num1, num2, 300));
end.
