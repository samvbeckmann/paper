program numbertest(input, output);
var num1: integer; var num2: integer;

procedure fibonacci(first: integer; second: integer; num: integer);
begin
    if num <= 0 then fibonacci := second;
    call fibonacci(second, first + second, num - 1)
end;

procedure gcd(first: integer; second: integer);
begin
    if second = 0 then gcd := first;
    call gcd(second, first mod second)
end;

procedure sumbelow(first: integer; second: real; max: integer);
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
    call read(num1, num2);
    call writeln(num1, 3);
    call writeln(5);
    call write(num1, num2);
    call writeln(7)
end.
