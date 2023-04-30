a,b = gets.chomp.split.map(&:to_i)

ans = gets.to_f

ans_bf = gets.to_f

print (ans-ans_bf).abs <= 1e-5 ? "ok" : "ng"