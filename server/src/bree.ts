const path = require('path');
const Bree = require('bree');

export const scheduleJob = (data?: any) => {
  const bree = new Bree({
    jobs: [
      {
        name: 'one-and-half-minute',
        path: path.join(__dirname, 'jobs', 'one-and-half-minute.js'),
        interval: '1.5 minutes',
        worker: {
          workerData: data,
        },
      },
    ],
  });

  console.log('Job scheduler started');
  bree.start('one-and-half-minute');
};
