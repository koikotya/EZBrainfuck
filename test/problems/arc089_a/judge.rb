n = gets.to_i
a = n.times.map{gets.chomp.split.map(&:to_i)}

ans = gets.chomp

ans_bf = gets.chomp

print ans == ans_bf ? "ok" : "ng"