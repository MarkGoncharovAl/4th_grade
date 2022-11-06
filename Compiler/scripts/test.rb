files = Dir.glob("#{File.dirname(__FILE__)}/../tests/*.mcl")
mcl = "#{File.dirname(__FILE__)}/../build/mcl"
sh = "#{File.dirname(__FILE__)}/test.sh"

out = []

files.each { |file|
  system("#{mcl} -f #{file} -o #{file}.ll")
  system("clang #{file}.ll --target=x86_64-unknown-linux-gnu -o #{file}.out")
  system("#{file}.out > #{file}.get")
  out.push( [system("diff #{file}.get #{file}.check"), file])
}

out.each { |pair|
  if pair[0]
    print ("PASSED: #{File.basename(pair[1])}\n")
  else
    print ("FAILED: #{File.basename(pair[1])}\n")
  end
}