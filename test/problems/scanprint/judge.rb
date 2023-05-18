n = 4
n_uint = gets.to_i
n_int = gets.to_i
n_fixed = gets.to_f
n_char = gets.chomp

ans = n.times.map{gets.chomp}

ans_bf = n.times.map{gets.chomp}

print ans == ans_bf ? "ok" : "ng"