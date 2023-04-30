a,b,c = gets.chomp.split.map(&:to_i)

ans = gets.to_i

ans_bf = gets.to_i

if ans == -1
    print ans_bf == -1 ? "ok" : "ng"
else
    print ans_bf%c == 0 && a <= ans_bf && ans_bf <= b ? "ok" : "ng"
end