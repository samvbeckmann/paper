program numbertest(input, output);
var num1: integer; var num2: integer;

procedure fibonacci(first: integer; second: integer; num: integer);
begin
    if num <= 0 then fibonacci := second
    else fibonacci := call fibonacci(second, first + second, num - 1)
end;

procedure gcd(first, second: integer);
begin
    if second = 0 then gcd := first
    else gcd := gcd(second, first mod second)
end;

procedure sumbelow(first, second, max: integer);
var temp:integer; var total: integer;
begin
    temp := 1;
    total := 0;
    while temp < max do
        begin
            if (temp mod first = 0) or (temp mod second = 0) then
                total := total + temp;
            temp := temp + 1
        end;
    sumbelow := total
end;

begin
    read(num1, num2);
    writeln(num1, fibonacci(0, 1, num1 - 1));
    writeln(gcd(num1, num2));
    write(num1, num2);
    writeln(sumbelow(num1, num2, 300));
end.
