Dir.glob("./problems/*") { |problem_path|
    next if !FileTest.directory?(problem_path)

    Dir.glob(File.join(problem_path, "/*.ezbf")) { |code|
        command = "./ezbf.exe " + code + " 2>/dev/null"
        system(command)
    }

    ok = true

    Dir.glob(File.join(problem_path, "/in/*")) { |in_path|
        out_path = in_path.gsub("in", "out")

        command = "bf -c999999999 ./a.bf < " + in_path
        output = IO.popen(command, "r+") { |io|
            io.readlines
        }

        out_file = File.open(out_path) { |f|
            f.readlines
        }

        in_file = File.open(in_path) { |f|
            f.readlines
        }

        command = "ruby " + problem_path + "/judge.rb"
        state = IO.popen(command, "r+") { |io|
            io.puts in_file
            io.puts out_file
            io.puts output
            io.close_write
            io.gets
        }

        if state != "ok"
            puts "#{problem_path}: ng"
            puts "error at testcase #{in_path}"
            puts "expected:", out_file
            puts "your output:", output
            ok = false
            break
        end
    }

    puts "#{problem_path}: ok" if ok
}