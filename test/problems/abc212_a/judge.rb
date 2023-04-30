a,b = gets.chomp.split.map(&:to_i)

ans = gets.chomp

ans_bf = gets.chomp

print ans == ans_bf ? "ok" : "ng"