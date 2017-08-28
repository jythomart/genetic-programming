function add(left, right) {
  return left + right;
}

function sub(left, right) {
  return left - right;
}

function mul(left, right) {
  return left * right;
}

function div(left, right) {
  return left / right;
}

function mod(left, right) {
  return left % right;
}

function cos(left, right) {
  return left * Math.cos(right);
}

function sin(left, right) {
  return left * Math.sin(right);
}

function min(left, right) {
  return Math.min(left, right);
}

function max(left, right) {
  return Math.max(left, right);
}


let firstLine = true;
let featureOffset = 3;

let predictions = '';

var lineReader = require('readline').createInterface({
  input: require('fs').createReadStream('numerai_tournament_data.csv')
});

lineReader.on('line', function (line) {
  if (firstLine) {
    console.log('id,probability');
    firstLine = false;
  } else {
    let items = line.split(',');
    predictions += items[0];
    predictions += ',';
    for (i = 0; i < 21; ++i) {
      global[`_${i}`] = parseFloat(items[i + featureOffset]);
      // console.log(`_${i} = `, global[`_${i}`]);
    }
    let predict = eval('add(div(mul(sub(_6, _4), min(mul(_18, max(_10, _11)), max(_7, _17))), add(_7, mul(0.511745, _3))), add(sub(_2, _18), max(_6, add(_9, _13))))');
    // console.log('predict = ', predict);
    if (isNaN(predict)) {
      console.error('NaN predict = ', predict)
      predict = 0.5;
    }
    if (predict < 0.0 || predict > 1.0)
      console.error('out of bound predict = ', predict)

    if (predict < 0.0)
      predict = 0.0 - predict;
    else if (predict > 1.0)
      predict = 2.0 - predict;

    predict = Math.min(Math.max(0.0, predict), 1.0);
    predictions += `${predict}\n`;
    console.log(`${items[0]},${predict}`);
  }
});

require('fs').writeFile('predictions.csv', predictions, (err) => {
  if (err) throw err;
});