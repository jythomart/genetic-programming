// ((_20 * (((_21 / (_40 - (_11 - ((_36 + -4.669076) + -0.066931)))) / (_12 + ((_35 / ((_31 / _34) * (_21 / _47))) + _13))) / (0.066070 + ((_19 + _2) - (_49 * _36))))) + 0.552240) = 0.690811
// ((_30 * (((_14 + _22) / ((_12 + (-2.582922 + ((_33 * _23) * (_25 + (_11 - _48))))) - _14)) * (_33 * _40))) + 0.564172) = 0.690826
// ((((_28 * ((_3 - _14) / (_22 * _14))) + (_32 * _32)) * (_16 * (_14 * _25))) + 0.475954)
// ((_30 * (((_39 + _22) / ((_28 + (-2.582922 - ((_12 * _0) * (_43 + (_19 * _49))))) + _14)) * (_15 * _40))) + 0.564172) = 0.687479
// ((_37 * (((_40 * _40) / ((_29 + (_44 - ((_19 - -3.042087) + (_45 * (_24 * _31))))) / _14)) / (_7 + _36))) + 0.527707) = 0.689556





// ((_24 / ((((_36 - (_0 + -4.334702)) + ((_15 / _5) - ((((_34 * (_40 * _21)) - ((_20 + _42) / (_25 * ((_3 - _15) + _36)))) + _0) / (_27 * _34)))) + ((((_18 - (_5 + _17)) - (_21 / _9)) + (_26 - _18)) - (_15 / (_15 / _5)))) - _45)) + 0.481427) = 0.687422


let firstLine = true;

let predictions = '';

var lineReader = require('readline').createInterface({
  input: require('fs').createReadStream('numerai_tournament_data.csv')
});

lineReader.on('line', function (line) {
  if (firstLine) {
    console.log('"t_id","probability"');
    firstLine = false;
  } else {
    let items = line.split(',');
    predictions += items[0];
    predictions += ',';
    for (i = 0; i < 50; ++i) {
      global[`_${i}`] = parseFloat(items[i + 1]);
      // console.log(`_${i} = `, global[`_${i}`]);
    }
    let predict = eval('((_24 / ((((_36 - (_0 + -4.334702)) + ((_15 / _5) - ((((_34 * (_40 * _21)) - ((_20 + _42) / (_25 * ((_3 - _15) + _36)))) + _0) / (_27 * _34)))) + ((((_18 - (_5 + _17)) - (_21 / _9)) + (_26 - _18)) - (_15 / (_15 / _5)))) - _45)) + 0.481427)');
    // console.log('predict = ', predict);
    if (isNaN(predict))
      predict = 0.5;
    predict = Math.min(Math.max(0.0, predict), 1.0);
    predictions += `${predict}\n`;
    console.log(`${items[0]},${predict}`);
  }
});

require('fs').writeFile('predictions.csv', predictions, (err) => {
  if (err) throw err;
});