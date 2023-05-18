n = 2
a = gets.to_i
b = gets.to_i

ans = n.times.map{gets.chomp}

ans_bf = n.times.map{gets.chomp}

print ans == ans_bf ? "ok" : "ng"