n = 5
l_uint = gets.to_i
r_uint = gets.to_i
l_int = gets.to_i
r_int = gets.to_i
l_fixed = gets.to_f
r_fixed = gets.to_f
lr_char = gets.chomp

ans = n.times.map{gets.chomp}

ans_bf = n.times.map{gets.chomp}

print ans == ans_bf ? "ok" : "ng"