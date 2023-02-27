const fs = require('fs')

let color = 'fill="#' + process.argv.slice(2)[0] + '"'
let replacement = process.argv.slice(3)
const dir = fs.opendirSync('.')
let directory_entry
while ((directory_entry = dir.readSync()) !== null) {
  if (directory_entry.name.startsWith('w') && directory_entry.name.endsWith('.svg')) {
    var output = directory_entry.name.replace('w', replacement)
    var data = fs.readFileSync(directory_entry.name).toString('utf-8').replace('fill="#fff"', color)
    fs.writeFileSync(output, data)
  }
}
dir.closeSync()
