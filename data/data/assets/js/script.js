const $ = document.querySelector.bind(document)
const label = [];
const datas = [];
for (let i=0; i < 2; i++) {
  for (let j=0; j < 60; j++) {
    label.push(`${i}:${j === 0 ? `00` : j}:`);
  }
}
var example = flatpickr('#flatpickr',{
  dateFormat: 'd-m-Y',
  onChange: function(selectedDates, dateStr, instance) {
      console.log(dateStr);
      reDrawChart([2,5,8,,1,,,5,5],label);
  },
});
function drawChart(datas,label) {
  let ctx1 = document.getElementById("induced").getContext("2d");
  let ctx2 = document.getElementById("external").getContext("2d");
    const myChart = new Chart(ctx1, {
        type: 'line',
        data: {
            labels: label,
            datasets: [{
                label: 'induced',
                data: datas,
  
                backgroundColor: 'rgb(255,255,255)',
  
            }],
  
        },
        options: {
            scales: {
                y: {
                    suggestedMin: 0.01,
                    suggestedMax: 100,
                    grid: {
                        borderColor: "white",
                        borderWidth: 2
                    }
                },
                x: {
                    grid: {
                        borderColor: "white",
                        borderWidth: 2
                    }
                }
  
            }
        }
    });
    const myChart2 = new Chart(ctx2, {
        type: 'bar',
        data: {
            labels: label,
            datasets: [{
                label: 'external',
                data: [12, 55, 3, 5, 2, 3],
                backgroundColor: 'rgb(255,255,255)'
            }]
        },
        options: {
            scales: {
                y: {
                    suggestedMin: 0.01,
                    suggestedMax: 100,
                    beginAtZero: true,
                    grid: {
                        borderColor: "white",
                        borderWidth: 2
                    }
                },
                x: {
                    grid: {
                        borderColor: "white",
                        borderWidth: 2
                    }
                }
  
            }
        }
    });
  }

function reDrawChart(data,label){
  
  $("canvas#external").remove();
  $("div.external-motion").innerHTML = '<canvas id="external" width="400" height="400"></canvas>';
  $("canvas#induced").remove();
  $("div.induced-motion").innerHTML = '<canvas id="induced" width="400" height="400"></canvas>';
  drawChart(data,label);
}
  



