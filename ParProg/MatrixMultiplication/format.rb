require 'optparse'
ARGV << '-h' if ARGV.empty?
options = OpenStruct.new
options.folder = ""

OptionParser.new do |opts|
  opts.banner = "Usage: #{$0} -f <folder>"
  opts.on("-f", "--folder f", "") { |v| options.folder = v }
end.parse!

