program errortest(input, output;
var : integer; var num : ;

procedure errortest(first, second: integer);
begin
    if first then second := first;
    errortest();
end
