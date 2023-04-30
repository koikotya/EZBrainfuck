a,b = gets.chomp.split.map(&:to_i)

ans = gets.to_f

ans_bf = gets.to_f

print (ans-ans_bf).abs <= 0.01 ? "ok" : "ng"